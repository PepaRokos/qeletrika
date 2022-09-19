CREATE TABLE "period" (
    "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "year" INTEGER NOT NULL,
    "month" INTEGER NOT NULL,
    "day" INTEGER NOT NULL
);

CREATE TABLE "prices" (
    "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "period" INTEGER NOT NULL,
    "hour" INTEGER NOT NULL,
    "price" NUMERIC,
    CONSTRAINT "period_fk"
            FOREIGN KEY ("period")
            REFERENCES "period" ("id")
);

CREATE TABLE "consumptions" (
    "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "period" INTEGER NOT NULL,
    "hour" INTEGER NOT NULL,
    "buy" NUMERIC,
    "sell" NUMERIC,
    CONSTRAINT "period_fk"
            FOREIGN KEY ("period")
            REFERENCES "period" ("id")
);

CREATE TABLE "courses" (
    "id" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
    "period" INTEGER NOT NULL,
    "course" NUMERIC,
    CONSTRAINT "period_fk"
            FOREIGN KEY ("period")
            REFERENCES "period" ("id")
);

CREATE VIEW "prices_view" AS
    select p.id, p.year, p.month, p.day, prices.hour, prices.price, (prices.price * c.course) as price_home from prices join period p on p.id = prices.period join courses c on p.id = c.period;

CREATE VIEW "hour_view" AS
    select p.year, p.month, p.day, p.hour, c.buy, c.sell, (c.buy * (price_home / 1000)) as spent, (c.sell * (price_home / 1000)) as earn from consumptions c join prices_view p on p.id = c.period where p.hour = c.hour;

CREATE VIEW "day_view" AS
    select hour_view.year, hour_view.month, hour_view.day, sum(hour_view.buy) as buy, sum(hour_view.sell) as sell, sum(hour_view.spent) as spent, sum(hour_view.earn) as earn from hour_view group by hour_view.year, hour_view.month, hour_view.day
