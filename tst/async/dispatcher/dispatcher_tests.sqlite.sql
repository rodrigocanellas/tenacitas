BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS "tests" (
	"test_id"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,
	"sent_expected"	INTEGER NOT NULL,
	"sent_done"	INTEGER NOT NULL,
	"handled_expected"	INTEGER NOT NULL,
	"handled_done"	INTEGER NOT NULL,
	"total_time"	INTEGER NOT NULL
);
CREATE TABLE IF NOT EXISTS "handlers" (
	"test_id"	INTEGER NOT NULL,
	"handling_id"	INTEGER NOT NULL,
	"handler_id"	INTEGER NOT NULL,
	"amount"	INTEGER NOT NULL,
	FOREIGN KEY("test_id","handling_id") REFERENCES "handlings",
	PRIMARY KEY("test_id","handling_id","handler_id")
);
CREATE TABLE IF NOT EXISTS "handlings" (
	"test_id"	INTEGER NOT NULL,
	"handling_id"	INTEGER NOT NULL,
	"sleep"	INTEGER NOT NULL,
	FOREIGN KEY("test_id") REFERENCES "tests"("test_id"),
	PRIMARY KEY("test_id","handling_id")
);
CREATE TABLE IF NOT EXISTS "generators" (
	"test_id"	INTEGER NOT NULL,
	"generator_id"	INTEGER NOT NULL,
	"amount"	INTEGER NOT NULL,
	"interval"	INTEGER NOT NULL,
	FOREIGN KEY("test_id") REFERENCES "tests"("test_id"),
	PRIMARY KEY("test_id","generator_id")
);
COMMIT;
