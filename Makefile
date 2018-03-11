all:
	g++ main.cpp CreateDisk.cpp Tables.cpp Utils.cpp Select.cpp BasicFileSystem.cpp Insert.cpp Update.cpp -o main.o
	./main.o

copypasta:
	create database kamil.dbo 200kb
	create table Empleado -columns=codigo,nombre,edad -types=int,char(100),double -key=codigo
	insert Empleado -columns=codigo,nombre,edad -values=122,el ingeniero me ha salvado ,09.485
	select Empleado -columns=codigo,nombre,edad -where=codigo<>1
	select Empleado -columns=* -where=codigo<>1
	update Empleado -columns=nombre,edad -values=’valor upd’,21.23 -where=codigo=500
