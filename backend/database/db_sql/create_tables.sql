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

-- book module data
CREATE TABLE IF NOT EXISTS rooms_all (
    id SERIAL PRIMARY KEY,
    room_number VARCHAR(10) NOT NULL UNIQUE,
    building VARCHAR(100),
    floor INT,
    total_area DECIMAL(5, 2),
    description TEXT,
    type VARCHAR(50) NOT NULL
);

CREATE TABLE IF NOT EXISTS lecture_room (
    room_id INT PRIMARY KEY REFERENCES rooms_all(id) ON DELETE CASCADE,
    has_projector BOOLEAN DEFAULT FALSE,
    has_whiteboard BOOLEAN DEFAULT TRUE,
    capacity INT NOT NULL
);

CREATE TABLE IF NOT EXISTS coworking_space (
    room_id INT PRIMARY KEY REFERENCES rooms_all(id) ON DELETE CASCADE,
    total_capacity INT NOT NULL,
    has_wifi BOOLEAN DEFAULT TRUE,
    has_printers BOOLEAN DEFAULT FALSE
);

CREATE TABLE IF NOT EXISTS private_office (
    room_id INT PRIMARY KEY REFERENCES rooms_all(id) ON DELETE CASCADE,
    number_of_chairs INT DEFAULT 2,
    has_phone BOOLEAN DEFAULT FALSE
);

CREATE TABLE IF NOT EXISTS room_availability (
    room_id INT REFERENCES rooms_all(id) ON DELETE CASCADE,
    date DATE NOT NULL,
    is_available BOOLEAN DEFAULT TRUE,
    available_from TIME DEFAULT '09:00',
    available_to TIME DEFAULT '18:00',
    booking_reference VARCHAR(50),
    UNIQUE (room_id, date, available_from, available_to)
);

CREATE TYPE booking_status AS ENUM (
    'pending',
    'confirmed',
    'cancelled'
);

CREATE TABLE IF NOT EXISTS room_booking (
    id SERIAL PRIMARY KEY,
    room_id INT REFERENCES rooms_all(id) ON DELETE CASCADE,
    user_id INT REFERENCES users(id) ON DELETE CASCADE,

    booking_date DATE NOT NULL,
    start_time TIME NOT NULL,
    end_time TIME NOT NULL,

    created_at TIMESTAMP DEFAULT NOW(),
    status booking_status DEFAULT 'confirmed'
);
