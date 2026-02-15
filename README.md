# RoomSched
Introduction for using postgresql in 'roomsched' project
1. sudo -u postgres psql -f create_database.sql
2. psql -U rsched_user -d roomsched -h localhost // Check connection
3. PGPASSWORD='...' psql -h localhost -U rsched_user -d roomsched -f create_tables.sql
4. Compile (temporary):  
g++-12 -std=c++20 -pedantic-errors -Wall -Wextra -Werror -fsanitize=undefined src/db_manager.cpp src/db_users.cpp db_main.cpp -o roomsched -lpqxx -lpq
5. Run programm: ./roomsched 