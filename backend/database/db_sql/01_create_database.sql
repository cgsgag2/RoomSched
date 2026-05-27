DO $$
BEGIN
    IF NOT EXISTS (
        SELECT FROM pg_catalog.pg_roles WHERE rolname = 'rsched_user'
    ) THEN
        CREATE USER rsched_user WITH PASSWORD 'RschedUser87204576';
    END IF;
END
$$;

GRANT ALL PRIVILEGES ON DATABASE roomsched TO rsched_user;
GRANT ALL ON SCHEMA public TO rsched_user;
