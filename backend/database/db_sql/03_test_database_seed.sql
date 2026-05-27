DO $$
BEGIN
    IF NOT EXISTS (
        SELECT FROM pg_catalog.pg_roles WHERE rolname = 'rsched_user_test'
    ) THEN
        CREATE USER rsched_user_test WITH PASSWORD 'password_tests_roomsched';
    END IF;
END
$$;

CREATE DATABASE roomsched_test OWNER rsched_user_test;
