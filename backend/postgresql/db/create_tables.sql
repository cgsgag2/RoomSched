CREATE TABLE IF NOT EXISTS users (
    id SERIAL PRIMARY KEY,
    username VARCHAR(50) UNIQUE NOT NULL,
    password_hash TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT NOW(),
    last_login TIMESTAMP
);

--TODO: continue working with them:
CREATE TABLE IF NOT EXISTS room_types (
    id SERIAL PRIMARY KEY,
    type_name VARCHAR(50) NOT NULL UNIQUE
);

CREATE TABLE IF NOT EXISTS room_info (
    id SERIAL PRIMARY KEY,
    room_number VARCHAR(10) NOT NULL UNIQUE,
    building VARCHAR(100),
    floor INT,
    total_area DECIMAL(5, 2),
    description TEXT,
    room_type_id INT NOT NULL REFERENCES room_types(id)
);

CREATE TABLE IF NOT EXISTS lecture_room (
    room_id INT PRIMARY KEY REFERENCES room_info(id) ON DELETE CASCADE,
    has_projector BOOLEAN DEFAULT FALSE,
    has_whiteboard BOOLEAN DEFAULT TRUE,
    capacity INT NOT NULL
);

CREATE TABLE IF NOT EXISTS coworking_space (
    room_id INT PRIMARY KEY REFERENCES room_info(id) ON DELETE CASCADE,
    total_capacity INT NOT NULL,
    has_wifi BOOLEAN DEFAULT TRUE,
    has_printers BOOLEAN DEFAULT FALSE
);

CREATE TABLE IF NOT EXISTS private_office (
    room_id INT PRIMARY KEY REFERENCES room_info(id) ON DELETE CASCADE,
    number_of_chairs INT DEFAULT 2,
    has_phone BOOLEAN DEFAULT FALSE
);

CREATE TABLE IF NOT EXISTS room_availability (
    id SERIAL PRIMARY KEY,
    room_id INT UNIQUE REFERENCES room_info(id),
    date DATE UNIQUE NOT NULL,
    is_available BOOLEAN DEFAULT TRUE,
    available_from TIME DEFAULT '09:00',
    available_to TIME DEFAULT '18:00',
    booking_reference VARCHAR(50)
);
