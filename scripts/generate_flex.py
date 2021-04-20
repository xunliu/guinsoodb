# use flex to generate the scanner file for the parser
# the following version of bison is used:
# flex 2.5.35 Apple(flex-32)
import os
import subprocess
import re
from python_helpers import open_utf8

pg_path = os.path.join('third_party', 'libpg_query')
flex_bin = 'flex'
flex_file_path = os.path.join(pg_path, 'scan.l')
target_file = os.path.join(pg_path, 'src_backend_parser_scan.cpp')

proc = subprocess.Popen([flex_bin, '--nounistd', '-o', target_file, flex_file_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
stdout = proc.stdout.read().decode('utf8')
stderr = proc.stderr.read().decode('utf8')
if proc.returncode != None or len(stderr) > 0:
	print("Flex failed")
	print("stdout: ", stdout)
	print("stderr: ", stderr)
	exit(1)

with open_utf8(target_file, 'r') as f:
	text = f.read()

# add the libpg_query namespace
text = text.replace('''
#ifndef FLEXINT_H
#define FLEXINT_H
''', '''
#ifndef FLEXINT_H
#define FLEXINT_H
namespace guinsoodb_libpgquery {
''')
text = text.replace('register ', '')

text = text + "\n} /* guinsoodb_libpgquery */\n"

text = re.sub('[(]void[)][ ]*fprintf', '//', text)
text = re.sub('exit[(]', 'throw std::runtime_error(msg); //', text)

if 'fprintf(' in text:
	print("PRINTF not removed!")
	# exit(1)

if 'exit(' in text:
	print("EXIT not removed!")
	# exit(1)

with open_utf8(target_file, 'w+') as f:
	f.write(text)
