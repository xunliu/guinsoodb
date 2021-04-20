test_that("guinsoodb_register() works", {
  con <- dbConnect(guinsoodb::guinsoodb())

  # most basic case
  guinsoodb::guinsoodb_register(con, "my_df1", iris)
  res <- dbReadTable(con, "my_df1")
  res$Species <- as.factor(res$Species)
  expect_true(identical(res, iris))
  guinsoodb::guinsoodb_unregister(con, "my_df1")

  guinsoodb::guinsoodb_register(con, "my_df2", mtcars)
  res <- dbReadTable(con, "my_df2")
  row.names(res) <- row.names(mtcars)
  expect_true(identical(res, mtcars))
  guinsoodb::guinsoodb_unregister(con, "my_df2")

  guinsoodb::guinsoodb_register(con, "my_df1", mtcars)
  res <- dbReadTable(con, "my_df1")
  row.names(res) <- row.names(mtcars)
  expect_true(identical(res, mtcars))

  # do not need unregister, can simply overwrite
  guinsoodb::guinsoodb_register(con, "my_df1", iris)
  res <- dbReadTable(con, "my_df1")
  res$Species <- as.factor(res$Species)
  expect_true(identical(res, iris))

  guinsoodb::guinsoodb_unregister(con, "my_df1")
  guinsoodb::guinsoodb_unregister(con, "my_df2")
  guinsoodb::guinsoodb_unregister(con, "xxx")

  # this needs to be empty now
  expect_true(length(attributes(con@conn_ref)) == 0)

  dbDisconnect(con, shutdown = T)
})


test_that("various error cases for guinsoodb_register()", {
  con <- dbConnect(guinsoodb::guinsoodb())

  guinsoodb::guinsoodb_register(con, "my_df1", iris)
  guinsoodb::guinsoodb_unregister(con, "my_df1")
  expect_error(dbReadTable(con, "my_df1"))

  expect_error(guinsoodb::guinsoodb_register(1, "my_df1", iris))
  expect_error(guinsoodb::guinsoodb_register(con, "", iris))
  expect_error(guinsoodb::guinsoodb_unregister(1, "my_df1"))
  expect_error(guinsoodb::guinsoodb_unregister(con, ""))
  dbDisconnect(con, shutdown = T)

  expect_error(guinsoodb::guinsoodb_unregister(con, "my_df1"))
})


test_that("uppercase data frames are queryable", {
  con <- dbConnect(guinsoodb::guinsoodb())
  guinsoodb::guinsoodb_register(con, "My_Mtcars", mtcars)
  dbGetQuery(con, "SELECT * FROM \"My_Mtcars\"")

  res <- dbReadTable(con, "My_Mtcars")
  row.names(res) <- row.names(mtcars)
  expect_true(identical(res, mtcars))
  guinsoodb::guinsoodb_unregister(con, "My_Mtcars")
  dbDisconnect(con, shutdown = T)
})
