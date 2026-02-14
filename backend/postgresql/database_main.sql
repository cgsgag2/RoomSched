CREATE DATABASE first_test_db;

\c first_test_db

CREATE TABLE test (
    message TEXT
);

INSERT INTO test (message)
VALUES ('First roomsched data base!');