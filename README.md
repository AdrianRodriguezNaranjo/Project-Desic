# Bus Application
The application arises from the need to have a platform where users can access information about bus lines, their schedules, and stops.

This application is being developed for the company Desic.

The idea of the application is as follows: through a login or registration system, users can access the application. Depending on the permissions associated with their login, they will have two ways to view the application:

### User:

The user will have the functions to view the currently established bus lines. From those lines, they can also check their schedules and stops.

### Administrator:

It has the same functions as the user, but with the additional capability to add, update, and delete any bus line, schedule, or stop.

# Data Model
In the following section, we will discuss the aspects of the entity-relationship and class diagram.

### Entity-relationship
![Entidad relacion](https://github.com/JoelMartinHernandez/Proyecto-Desic/assets/131310480/53c42eb9-d4bf-418d-b028-d10c292cd152)
The "Users" table contains the fields ID, username, password, name, and discriminator(this controls the roles of the users)

The "Lines" table contains the ID, number, firstBusStop, LastBusStop

The "Users_Lines" table is created based on the many-to-many relationship between the "Users" and "Lines" tables. This table contains "UsersID," which is the primary key of the "Users" table, and "LinesID," which is the primary key of the "Lines" table.

The "Schedule" table contains the fields ID, HourAndMinutes, and LinesID, which is the primary key of the "Lines" table.

The "BusStops" table contains the fields ID, Location, and LinesID, which is the primary key of the "Lines" table.

### Class diagram
![Diagrama de clases2 Desic](https://github.com/JoelMartinHernandez/Proyecto-Desic/assets/131310480/68aa25d2-4986-41cc-8687-fc7c62cd539f)

### Use cases
![Diagrama de casos](https://github.com/JoelMartinHernandez/Proyecto-Desic/assets/131310480/20ddb853-f22f-4040-b69c-d6bc75a1658d)

# Endpoints

# User requirement
The aplication works in web in particular on mobile.
> [!IMPORTANT]
> It is possible to use the application on desktop but it is not intended for desktop use.

### 1.User:
1.1 Any user need register into the aplication.

1.2 If you want to see the stops or the schedule, first select the line.

### 2.Administrator:
2.1 All the registers need the elements that are requested. that includes images.

2.2 The user with the role administrator has to be assigned in the database.

2.3 When deleting a line, stops and times are deleted in cascade.

# Technical specifications for the server application and the APP
The application uses a mysql server to store the logs of all tables and a backend folder in public/images to store the images themselves.

In the application, if a record containing a photo is deleted, it is already programmed to delete the photo in the backend as well.
> [!WARNING]
>In case the backend closes normally or unexpectedly, there is no way to navigate through the frontend.

# Figma
[Enlace Figma](https://www.figma.com/file/Qs5ePGQ6fIWDLK2SWQhob9/Proyecto-Desic?type=design&node-id=0%3A1&mode=design&t=rI9LVburwdWDxmGZ-1)

# Usability and Accessibility

# Manuals

# User manual

# Technological Stack
### Backend:
  * Qt
  * C++
  * PostgresSQL
  
### Frontend:
  * React
  * JavaScript
  * HTML5
  * CSS3

# type of application

# Why this type of application?

# Repository
 ![GitHub]
 [Enlace del GitHub](https://github.com/Naidr/Project-Desic.git)

 # Planning

To plan my project, I have used the Scrum methodology.

# Conclusions

The application, although it may seem basic, provides users with the essential information they need when looking for basic bus information. It is quite intuitive for administrators as well.

The project has contributed to my growth as a programmer, allowing me to develop new knowledge about backends and React. I gained insights into topics such as tokens and private addresses.


[Github]:https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white
[Github-url]: https://github.com/Naidr/Project-Desic.git
