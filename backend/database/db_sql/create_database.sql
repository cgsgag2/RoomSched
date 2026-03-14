CREATE DATABASE roomsched;
\c roomsched

--Create app user
CREATE USER rsched_user WITH PASSWORD 'RschedUser87204576';
GRANT ALL PRIVILEGES ON DATABASE roomsched TO rsched_user;

ALTER DATABASE roomsched OWNER TO rsched_user;
GRANT ALL ON SCHEMA public TO rsched_user;
