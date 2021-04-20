DBDIR_MEMORY <- ":memory:"

check_flag <- function(x) {
  if (is.null(x) || length(x) != 1 || is.na(x) || !is.logical(x)) {
    stop("flags need to be scalar logicals")
  }
}

extptr_str <- function(e, n = 5) {
  x <- .Call(guinsoodb_ptr_to_str, e)
  substr(x, nchar(x) - n + 1, nchar(x))
}

drv_to_string <- function(drv) {
  if (!is(drv, "guinsoodb_driver")) {
    stop("pass a guinsoodb_driver object")
  }
  sprintf("<guinsoodb_driver %s dbdir='%s' read_only=%s>", extptr_str(drv@database_ref), drv@dbdir, drv@read_only)
}

#' @rdname guinsoodb_driver-class
#' @inheritParams methods::show
#' @export
setMethod(
  "show", "guinsoodb_driver",
  function(object) {
    message(drv_to_string(object))
    invisible(NULL)
  }
)

#' Connect to a GuinsooDB database instance
#'
#' `dbConnect()` connects to a database instance.
#'
#' @param drv Object returned by `guinsoodb()`
#' @param dbdir Location for database files. Should be a path to an existing
#'   directory in the file system. With the default, all
#'   data is kept in RAM
#' @param ... Ignored
#' @param debug Print additional debug information such as queries
#' @param read_only Set to `TRUE` for read-only operation
#' @param timezone_out The time zone returned to R, defaults to `"UTC"`, which
#'   is currently the only timezone supported by guinsoodb.
#'   If you want to display datetime values in the local timezone,
#'   set to [Sys.timezone()] or `""`.
#' @param tz_out_convert How to convert timestamp columns to the timezone specified
#'   in `timezone_out`. There are two options: `"with"`, and `"force"`. If `"with"`
#'   is chosen, the timestamp will be returned as it would appear in the specified time zone.
#'   If `"force"` is chosen, the timestamp will have the same clock
#'   time as the timestamp in the database, but with the new time zone.
#'
#' @return `dbConnect()` returns an object of class
#'   \linkS4class{guinsoodb_connection}.
#'
#' @rdname guinsoodb
#' @export
#' @examples
#' drv <- guinsoodb()
#' con <- dbConnect(drv)
#'
#' dbGetQuery(con, "SELECT 'Hello, world!'")
#'
#' dbDisconnect(con)
#' guinsoodb_shutdown(drv)
#'
#' # Shorter:
#' con <- dbConnect(guinsoodb())
#' dbGetQuery(con, "SELECT 'Hello, world!'")
#' dbDisconnect(con, shutdown = TRUE)
setMethod(
  "dbConnect", "guinsoodb_driver",
  function(drv, dbdir = DBDIR_MEMORY, ...,
           debug = getOption("guinsoodb.debug", FALSE),
           read_only = FALSE,
           timezone_out = "UTC",
           tz_out_convert = c("with", "force")) {

    check_flag(debug)
    timezone_out <- check_tz(timezone_out)
    tz_out_convert <- match.arg(tz_out_convert)

    missing_dbdir <- missing(dbdir)
    dbdir <- path.expand(as.character(dbdir))

    # aha, a late comer. let's make a new instance.
    if (!missing_dbdir && dbdir != drv@dbdir) {
      guinsoodb_shutdown(drv)
      drv <- guinsoodb(dbdir, read_only)
    }

    conn <- guinsoodb_connection(drv, debug = debug)
    on.exit(dbDisconnect(conn))

    conn@timezone_out <- timezone_out
    conn@tz_out_convert <- tz_out_convert

    on.exit(NULL)

    if (!isTRUE(debug)){
      # only has effect in RStudio
      on_connection_opened(conn)
    }

    conn
  }
)

#' @description
#' `dbDisconnect()` closes a GuinsooDB database connection, optionally shutting down
#' the associated instance.
#'
#' @param conn A `guinsoodb_connection` object
#' @param shutdown Set to `TRUE` to shut down the GuinsooDB database instance that this connection refers to.
#' @rdname guinsoodb
#' @export
setMethod(
  "dbDisconnect", "guinsoodb_connection",
  function(conn, ..., shutdown = FALSE) {
    if (!dbIsValid(conn)) {
      warning("Connection already closed.", call. = FALSE)
    }
    .Call(guinsoodb_disconnect_R, conn@conn_ref)
    if (shutdown) {
      guinsoodb_shutdown(conn@driver)
    }
    on_connection_closed(conn)
    invisible(TRUE)
  }
)

#' @description
#' `guinsoodb()` creates or reuses a database instance.
#'
#' @return `guinsoodb()` returns an object of class \linkS4class{guinsoodb_driver}.
#'
#' @import methods DBI
#' @export
guinsoodb <- function(dbdir = DBDIR_MEMORY, read_only = FALSE) {
  check_flag(read_only)
  new(
    "guinsoodb_driver",
    database_ref = .Call(guinsoodb_startup_R, dbdir, read_only),
    dbdir = dbdir,
    read_only = read_only
  )
}

#' @rdname guinsoodb_driver-class
#' @export
setMethod(
  "dbDataType", "guinsoodb_driver",
  function(dbObj, obj, ...) {

    if (is.null(obj)) stop("NULL parameter")
    if (is.data.frame(obj)) {
      return(vapply(obj, function(x) dbDataType(dbObj, x), FUN.VALUE = "character"))
    }
    #  else if (int64 && inherits(obj, "integer64")) "BIGINT"
    else if (inherits(obj, "Date")) {
      "DATE"
    } else if (inherits(obj, "difftime")) {
      "TIME"
    } else if (is.logical(obj)) {
      "BOOLEAN"
    } else if (is.integer(obj)) {
      "INTEGER"
    } else if (is.numeric(obj)) {
      "DOUBLE"
    } else if (inherits(obj, "POSIXt")) {
      "TIMESTAMP"
    } else if (is.list(obj) && all(vapply(obj, typeof, FUN.VALUE = "character") == "raw" || is.na(obj))) {
      "BLOB"
    } else {
      "STRING"
    }

  }
)

#' @rdname guinsoodb_driver-class
#' @inheritParams DBI::dbIsValid
#' @importFrom DBI dbConnect
#' @export
setMethod(
  "dbIsValid", "guinsoodb_driver",
  function(dbObj, ...) {
    valid <- FALSE
    tryCatch(
      {
        con <- dbConnect(dbObj)
        dbExecute(con, SQL("SELECT 1"))
        dbDisconnect(con)
        valid <- TRUE
      },
      error = function(c) {
      }
    )
    valid
  }
)

#' @rdname guinsoodb_driver-class
#' @inheritParams DBI::dbGetInfo
#' @export
setMethod(
  "dbGetInfo", "guinsoodb_driver",
  function(dbObj, ...) {
    con <- dbConnect(dbObj)
    version <- dbGetQuery(con, "select library_version from pragma_version()")[[1]][[1]]
    dbDisconnect(con)
    list(driver.version = version, client.version = version, dbname=dbObj@dbdir)
  }
)


#' @description
#' `guinsoodb_shutdown()` shuts down a database instance.
#'
#' @return `dbDisconnect()` and `guinsoodb_shutdown()` are called for their
#'   side effect.
#' @rdname guinsoodb
#' @export
guinsoodb_shutdown <- function(drv) {
  if (!is(drv, "guinsoodb_driver")) {
    stop("pass a guinsoodb_driver object")
  }
  if (!dbIsValid(drv)) {
    warning("invalid driver object, already closed?")
    invisible(FALSE)
  }
  .Call(guinsoodb_shutdown_R, drv@database_ref)
  invisible(TRUE)
}

is_installed <- function(pkg) {
  as.logical(requireNamespace(pkg, quietly = TRUE)) == TRUE
}

check_tz <- function(timezone) {

  if (!is.null(timezone) && timezone == "") {
    return(Sys.timezone())
  }

  if (is.null(timezone) || !timezone %in% OlsonNames()) {
    warning(
      "Invalid time zone '", timezone, "', ",
      "falling back to UTC.\n",
      "Set the `timezone_out` argument to a valid time zone.\n",
      call. = FALSE
    )
    return("UTC")
  }

  timezone
}
