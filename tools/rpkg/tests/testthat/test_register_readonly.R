test_that("we can register a data frame on a read only connection", {
  path = tempfile()
  # create empty database
  con = DBI::dbConnect(guinsoodb::guinsoodb(), dbdir = path, read_only = FALSE)
  DBI::dbDisconnect(con, shutdown = TRUE)

  # reopen database read-only, try to write temp table
  con = DBI::dbConnect(guinsoodb::guinsoodb(), dbdir = path, read_only = TRUE)
  guinsoodb::guinsoodb_register(con, "mtcars", mtcars)
})

