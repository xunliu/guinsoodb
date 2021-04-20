#include "httpfs.hpp"
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.hpp"

#include <map>

using namespace guinsoodb;

unique_ptr<ResponseWrapper> HTTPFileSystem::Request(FileHandle &handle, string url, string method, HeaderMap header_map,
                                                    idx_t file_offset, char *buffer_out, idx_t buffer_len) {
	auto headers = make_unique<httplib::Headers>();
	for (auto &entry : header_map) {
		headers->insert(entry);
	}
	if (url.rfind("http://", 0) != 0 && url.rfind("https://", 0) != 0) {
		throw std::runtime_error("URL needs to start with http:// or https://");
	}
	auto slash_pos = url.find('/', 8);
	if (slash_pos == std::string::npos) {
		throw std::runtime_error("URL needs to contain a '/' after the host");
	}
	auto proto_host_port = url.substr(0, slash_pos);

	auto path = url.substr(slash_pos);
	if (path.empty()) {
		throw std::runtime_error("URL needs to contain a path");
	}

	httplib::Client cli(proto_host_port.c_str());
	cli.set_follow_location(true);
	cli.enable_server_certificate_verification(false);

	if (method == "HEAD") {
		auto res = cli.Head(path.c_str(), *headers);
		if (res.error() != httplib::Error::Success) {
			throw std::runtime_error("HTTP HEAD error on '" + url + "' " + std::to_string(res.error()));
		}
		return make_unique<ResponseWrapper>(res.value());
	}
	std::string range_expr =
	    "bytes=" + std::to_string(file_offset) + "-" + std::to_string(file_offset + buffer_len - 1);
	// printf("%s(%llu, %llu)\n", method.c_str(), file_offset, buffer_len);

	// send the Range header to read only subset of file
	headers->insert(std::pair<std::string, std::string>("Range", range_expr));

	idx_t out_offset = 0;
	auto res = cli.Get(
	    path.c_str(), *headers,
	    [&](const httplib::Response &response) {
		    if (response.status >= 400) {
			    throw std::runtime_error("HTTP error");
		    }
		    if (response.status < 300) { // done redirectering
			    out_offset = 0;
			    auto content_length = std::stoll(response.get_header_value("Content-Length", 0));
			    if ((idx_t)content_length != buffer_len) {
				    throw std::runtime_error("offset error");
			    }
		    }
		    return true;
	    },
	    [&](const char *data, size_t data_length) {
		    memcpy(buffer_out + out_offset, data, data_length);
		    out_offset += data_length;
		    return true;
	    });
	if (res.error() != httplib::Error::Success) {
		throw std::runtime_error("HTTP GET error on '" + url + "' " + std::to_string(res.error()));
	}
	return make_unique<ResponseWrapper>(res.value());
}

HTTPFileHandle::HTTPFileHandle(FileSystem &fs, std::string path)
    : FileHandle(fs, path), length(0), buffer_available(0), buffer_idx(0), file_offset(0) {
	buffer = std::unique_ptr<data_t[]>(new data_t[BUFFER_LEN]);

	IntializeMetadata();
}

std::unique_ptr<FileHandle> HTTPFileSystem::OpenFile(const char *path, uint8_t flags, FileLockType lock) {
	return guinsoodb::make_unique<HTTPFileHandle>(*this, path);
}

void HTTPFileSystem::Read(FileHandle &handle, void *buffer, int64_t nr_bytes, idx_t location) {
	auto &hfh = (HTTPFileHandle &)handle;
	idx_t to_read = nr_bytes;
	idx_t buffer_offset = 0;
	if (location + nr_bytes > hfh.length) {
		throw std::runtime_error("out of file");
	}

	// TODO we need to check if location is within the cached buffer and update or invalidate
	// for now just invalidate
	if (location != hfh.file_offset) {
		hfh.buffer_available = 0;
		hfh.buffer_idx = 0;
		hfh.file_offset = location;
	}

	while (to_read > 0) {
		auto buffer_read_len = MinValue<idx_t>(hfh.buffer_available, to_read);
		memcpy((char *)buffer + buffer_offset, hfh.buffer.get() + hfh.buffer_idx, buffer_read_len);

		buffer_offset += buffer_read_len;
		to_read -= buffer_read_len;

		hfh.buffer_idx += buffer_read_len;
		hfh.buffer_available -= buffer_read_len;
		hfh.file_offset += buffer_read_len;

		if (to_read > 0 && hfh.buffer_available == 0) {
			auto new_buffer_available = MinValue<idx_t>(hfh.BUFFER_LEN, hfh.length - hfh.file_offset);
			Request(hfh, hfh.path, "GET", {}, hfh.file_offset, (char *)hfh.buffer.get(), new_buffer_available);
			hfh.buffer_available = new_buffer_available;
			hfh.buffer_idx = 0;
		}
	}
}

void HTTPFileHandle::IntializeMetadata() {
	// get length using HEAD
	auto &hfs = (HTTPFileSystem &)file_system;
	auto res = hfs.Request(*this, path, "HEAD");
	if (res->code != 200) {
		throw std::runtime_error("Unable to connect " + res->error);
	}
	length = std::atoll(res->headers["Content-Length"].c_str());

	struct tm tm;
	strptime(res->headers["Last-Modified"].c_str(), "%a, %d %h %Y %T %Z", &tm);
	last_modified = std::mktime(&tm);
}

ResponseWrapper::ResponseWrapper(httplib::Response &res) {
	code = res.status;
	error = res.body;
	for (auto &h : res.headers) {
		headers[h.first] = h.second;
	}
}
