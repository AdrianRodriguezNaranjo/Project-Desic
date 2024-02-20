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
### The "Users" table contains the fields ID, username, password, name, and discriminator(this controls the roles of the users)
this table contains the information of the users cas name, password and username.

### The "Lines" table contains the ID, number, firstBusStop, LastBusStop
the table line contains the information of each line and what is the Started bus stop and what is the last stop od this line.

### The "Users_Lines" table is created based on the many-to-many relationship between the "Users" and "Lines" tables. This table contains "UsersID," which is the primary key of the "Users" table, and "LinesID," which is the primary key of the "Lines" table.
this one is the table with relaciona the table user and lines by the relationship of many to many.

### The "Schedule" table contains the fields ID, HourAndMinutes, and LinesID, which is the primary key of the "Lines" table.
this table contains the schedule of each line showing the hour.

### The "BusStops" table contains the fields ID, Location, and LinesID, which is the primary key of the "Lines" table.
this table contains the stops of each line showing the location

### Class diagram
![Diagrama de clases2 Desic](https://github.com/JoelMartinHernandez/Proyecto-Desic/assets/131310480/68aa25d2-4986-41cc-8687-fc7c62cd539f)

### Use cases
![Diagrama de casos](https://github.com/JoelMartinHernandez/Proyecto-Desic/assets/131310480/20ddb853-f22f-4040-b69c-d6bc75a1658d)

# Endpoints
[Postman](https://documenter.getpostman.com/view/29808712/2sA2r9Wi9v)

# User requirement
The aplication aprobe with functions to navigate in each lines, explore the schedule and the bus stop.

This aplication is designed to the administrator, but the users can use the aplication normaly without the administrator role.

The aplication works in web in particular on mobile.
> [!IMPORTANT]
> It is possible to use the application on desktop but it is not intended for desktop use.

### 1.User:
1.1 Any user need register into the aplication.

1.2 If you want to see the stops or the schedule, first select the line.

### 2.Administrator:
2.1 All the registers need the elements that are requested.

2.2 The user with the role administrator has to be assigned in the database.

2.3 When deleting a line, stops and times are deleted in cascade.

# Technical specifications for the server application and the APP
> [!WARNING]
>In case the backend closes normally or unexpectedly, there is no way to navigate through the frontend.

# Figma
[Enlace Figma](https://www.figma.com/file/Qs5ePGQ6fIWDLK2SWQhob9/Proyecto-Desic?type=design&node-id=0%3A1&mode=design&t=rI9LVburwdWDxmGZ-1)

# Usability and Accessibility

# Manuals
## Developer installation
Before executing the file it is necessary to have** postgresSql installed** and ** QT **.

Once the above is done, it is time to install the backend. Located in the Backend folder of the project, we go to run the Backend. This action will create the databases tables automatic in the database named before corresponding

Now to install the Frontend, follow the same steps as above, but instead of going to the Backend folder, this time we go to the **frontend folder** and use the comand "**npm install**" step.

If you now want to start everything you need to execute the backend with Qt, and the frontend with Visual Studio Code through a terminal with Git bash for example, run the command "**npm start**".\

When you run npm start on the frontend, the application will be opened.
> [!NOTE]
> When you open only the application without the backend, as it is a web application, it is normal that it does not work.


## User/Technician installation
To install the application, it is necessary to access the Frontend folder of the project folder.\
Once accessed the folder, enter the command in the terminal "**npm install**" to install the necessary files, and after installing everything run the command "**npm start**" to open the application.

# User manual
To begin using the application, it is necessary to have a registered account. If you don't have one, you can click on 'Register' to sign up for our application. If you already have an account, you can use 'Log In' with your existing credentials.

The application is divided into 3 main sections:

1. Lines
   - In the main view, you will see all the lines that are currently in operation.
2. Schedule
   - By selecting a line, you can view the departure times associated with the selected line in this view.
3. Stops
   - By selecting a line, you can view the stops along with their images associated with the selected line in this view.

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
I use this format because usually the information of each line and stop is updated every few days so it is better to rely on a server. Then another point is that as the information is constantly changing, it is not profitable to store information locally, making the application heavier.

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
