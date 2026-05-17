#
# Tests utils module (e2e python test).
#

import requests

BASE_URL = "http://localhost:8080"

def register_user(email, password, full_name, phone):
    return requests.post(
        f"{BASE_URL}/register",
        json={
            "email": email,
            "password": password,
            "full_name": full_name,
            "phone": phone,
        },
    )

def login_user(email, password):
    return requests.post(
        f"{BASE_URL}/login",
        json={
            "email": email,
            "password": password,
        },
    )

def create_room(name, capacity):
    return requests.post(
        f"{BASE_URL}/rooms",
        json={
            "name": name,
            "capacity": capacity,
        },
    )
