DO $$
BEGIN
    IF NOT EXISTS (SELECT FROM pg_catalog.pg_user WHERE usename = 'rsched_user_test') THEN
        CREATE USER rsched_user_test WITH PASSWORD 'password_tests_roomsched';
    END IF;
END
$$;

DROP DATABASE IF EXISTS roomsched_test;
CREATE DATABASE roomsched_test OWNER rsched_user_test;
\c roomsched_test

SET ROLE rsched_user_test;

-- user data
CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    email VARCHAR(100) UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    full_name VARCHAR(50) NOT NULL,
    phone VARCHAR(20) UNIQUE NOT NULL,
    role VARCHAR(20) DEFAULT 'user',
    created_at TIMESTAMP DEFAULT NOW(),
    last_login TIMESTAMP
);

SET ROLE postgres;
GRANT ALL ON SCHEMA public TO rsched_user_test;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO rsched_user_test;
GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO rsched_user_test;
