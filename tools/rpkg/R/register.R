#' Register a data frame as a virtual table
#'
#' `guinsoodb_register()` registers a data frame as a virtual table (view)
#'  in a GuinsooDB connection.
#'  No data is copied.
#'
#' `guinsoodb_unregister()` unregisters a previously registered data frame.
#' @param conn A GuinsooDB connection, created by `dbConnect()`.
#' @param name The name for the virtual table that is registered or unregistered
#' @param df A `data.frame` with the data for the virtual table
#' @return These functions are called for their side effect.
#' @export
#' @examples
#' con <- dbConnect(guinsoodb())
#'
#' data <- data.frame(a = 1:3, b = letters[1:3])
#'
#' guinsoodb_register(con, "data", data)
#' dbReadTable(con, "data")
#'
#' guinsoodb_unregister(con, "data")
#' try(dbReadTable(con, "data"))
#'
#' dbDisconnect(con)
guinsoodb_register <- function(conn, name, df) {
  stopifnot(dbIsValid(conn))
  .Call(guinsoodb_register_R, conn@conn_ref, as.character(name), as.data.frame(df))
  invisible(TRUE)
}

#' @rdname guinsoodb_register
#' @export
guinsoodb_unregister <- function(conn, name) {
  stopifnot(dbIsValid(conn))
  .Call(guinsoodb_unregister_R, conn@conn_ref, as.character(name))
  invisible(TRUE)
}

