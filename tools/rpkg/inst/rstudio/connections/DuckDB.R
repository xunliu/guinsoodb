library(DBI)
library(guinsoodb)

# create / connect to database file
drv <- guinsoodb(dbdir = "${1:database=quack.db}")
con <- dbConnect(drv)

## write a table to it
# dbWriteTable(con, "iris", iris)

## and disconnect
# dbDisconnect(con, shutdown=TRUE)
