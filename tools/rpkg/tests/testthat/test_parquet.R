test_that("parquet reader works on the notorious userdata1 file", {
  con <- dbConnect(guinsoodb::guinsoodb())
  res <- dbGetQuery(con, "SELECT * FROM parquet_scan('userdata1.parquet')")
  dbDisconnect(con, shutdown = T)

  expect_true(T)
})
