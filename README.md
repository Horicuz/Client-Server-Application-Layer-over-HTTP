# Assignment 3 PCOM - REST Client

## General Description

This project implements a client that connects to a server for managing an online movie library.  
The client allows both administrator and regular user authentication and supports user, movie, and collection management.

---

## Project Structure

The project is organized into multiple files to ensure clear structure and proper separation of responsibilities:

```
├── client.cpp
├── helper.h/.cpp
├── requests.h/.cpp
├── functions.h/.cpp
└── json.hpp
```

- **client.cpp**: The entry point of the application, managing the user interface and command flow  
- **helper.h/cpp**: Contains utility functions for handling connections and HTTP messages  
- **requests.h/cpp**: Implements the main HTTP request types (GET, POST, DELETE, PUT)  
- **functions.h/cpp**: Implements the core functionality (user, movie, and collection management)  
- **json.hpp**: External library used for JSON manipulation  

---

## Main Components

### 1. HTTP Communication System

Implemented functions for all required HTTP methods:
- `compute_get_request`: For retrieving resources (GET)
- `compute_json_post_request`: For creating new resources (POST)
- `compute_delete`: For deleting resources (DELETE)
- `compute_json_put_request`: For updating resources (PUT)

These functions build properly formatted HTTP messages with all required headers.

---

### 2. Connection Management

The `Helper` class provides functionalities for:
- Opening and closing TCP connections  
- Sending and receiving messages via socket  
- Managing buffers for HTTP message handling  

---

### 3. Authentication and Authorization

The system implements:
- Authentication for both administrators and users  
- Session cookie management  
- Authorization support using JWT tokens  

---

### 4. Core Operations

The client supports the following operations:
- **For administrators:** adding, deleting, and listing users  
- **For regular users:** accessing the movie library and managing their movie collections  

---

### 5. JSON Parsing

For JSON manipulation, the project uses the [nlohmann/json](https://github.com/nlohmann/json) library because:
- It is a **header-only** library (easy to integrate)  
- Provides **intuitive and modern syntax**  
- Fully supports **serialization and deserialization** of JSON objects  
- Is **well-documented** and **actively maintained**  
- Offers **optimized performance** for modern C++  

---

## Implemented Functionalities

### Authentication

The system supports two types of authentication:
1. **Administrator authentication:** allows user management  
2. **User authentication:** allows access to the movie library and collection management  

After login, session cookies are stored for subsequent requests.

---

### Movie Management

Authenticated users can:
- View the list of available movies  
- Add new movies to the system  
- Delete existing movies  
- Update movie information  

---

### Collection Management

Users can create and manage personalized movie collections:
- Create new collections  
- Add movies to collections  
- View existing collections  
- Remove movies from collections  
- Delete entire collections  

---

## Compilation and Execution

To compile the project, use:

```bash
make
```

To run the client:

```bash
make run
```

or simply:

```bash
./client
```

---

## Available Commands

The client supports the following commands:

### Administrator commands
- `login_admin` – Log in as administrator  
- `add_user` – Add a new user (admin only)  
- `delete_user` – Delete an existing user (admin only)  
- `get_users` – List all users (admin only)  
- `logout_admin` – Log out administrator  

### User commands
- `login` – Log in as regular user  
- `logout` – Log out user  
- `get_access` – Obtain JWT token for accessing the movie library  

### Movie commands
- `get_movies` – List all movies  
- `get_movie` – Display details for a specific movie  
- `add_movie` – Add a new movie  
- `delete_movie` – Delete a movie  
- `update_movie` – Update movie information  

### Collection commands
- `get_collections` – List all collections  
- `get_collection` – Show details for a specific collection  
- `add_collection` – Create a new collection  
- `delete_collection` – Delete a collection  
- `add_movie_to_collection` – Add a movie to a collection  
- `delete_movie_from_collection` – Remove a movie from a collection  

---

## Error Handling

The application includes extensive validation and proper error handling for:
- User input data  
- Server error responses  
- Connection issues  

Each operation checks the current session state and provides descriptive error messages when needed.
