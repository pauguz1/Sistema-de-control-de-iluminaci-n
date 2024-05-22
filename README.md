# Sistema-de-control-de-iluminacion⚡
Sistema de control de iluminación utilizando microservicios

# Microservicios

# microservicio-1
Es el encargado de la redireccion de los mensajes entre los usuarios
y el microcontrolador

# microservicio-2
Es el encargado de la presentacion de la aplicacion al usuario mediante
una pagina web

<br>

# Instalación

### 1 Construir la imagen de docker
```docker
sudo docker-compose build
```
### 2 Correr el contenedor
```docker
sudo docker-compose up
```
### 3 Subir al codigo al microcontrolador
Se debe grabar el código del microcontrolador para que este se
conecte a los microservicios y con ello poder controlar la lámpara.
```arduino
ubiacion del codigo
./codigo_microcontrolador/codigo_microcontrolador.ino
```

### 3.1 Activar el simulador de microcontrolador (solo si quieres probar sin utilizar un microcontrolador)
Para ejecutar un microcontrolador simulado solo debes ejecutar el archivo microcontrolador.html el cual simulará ser un microcontrolador con una lámpara.
```arduino
ubiacion del simulador
./microservicio-1/microcontrolador.html
```

<br>
<br>

### Visita mi página web para que veas mas sobre mis proyectos y mas sobre mi

# [Web Paul Santana](https://pauguz1.github.io/Paul_Santana/)
