# Recomendador de Libros para Biblioteca

Sistema de recomendación de libros implementado en C++ usando un grafo dirigido y ponderado con búsqueda en anchura (BFS).

## Descripción

El programa representa una biblioteca de 50 libros como un grafo donde cada nodo es un libro y cada arista conecta dos libros que han sido prestados en secuencia por los usuarios. A partir del último libro prestado por un usuario, el sistema recorre el grafo y sugiere los 5 libros más relevantes según el historial de copréstamos, el género favorito del usuario y la calificación promedio de cada libro.

## Estructura del proyecto

```
biblioteca/
├── recomendador_libros.cpp   # Código fuente completo
└── README.md
```

## Requisitos

- Compilador g++ con soporte para C++14 o superior.

## Compilación y ejecución

**Linux / Mac**
```bash
g++ -std=c++14 -o recomendador recomendador_libros.cpp
./recomendador
```

**Windows**
```bash
g++ -std=c++14 -o recomendador recomendador_libros.cpp
recomendador.exe
```

**OnlineGDB** — Pega el contenido de `recomendador_libros.cpp` en el editor, selecciona C++ como lenguaje y presiona Run.

## Menú principal

```
1. Ver catálogo de libros
2. Registrar nuevo usuario
3. Seleccionar usuario activo
4. Registrar préstamo
5. Obtener recomendaciones
6. Ver historial de préstamos
7. Calificar un libro
0. Salir
```

## Ejemplo de uso rápido

Al iniciar, el programa carga automáticamente un usuario **Demo** con el libro *Dune* en su historial. Para probar las recomendaciones de inmediato selecciona la opción `5` y luego `1`.

Para registrar un usuario propio selecciona la opción `2`, ingresa tu nombre, elige tus géneros favoritos, el rango de años y la calificación mínima aceptable. Luego usa la opción `4` para registrar un préstamo y la opción `5` para obtener tus recomendaciones personalizadas.

## Géneros disponibles

| # | Género |
|---|--------|
| 1 | Ciencia Ficción |
| 2 | Fantasía |
| 3 | Clásico |
| 4 | Thriller |
| 5 | Historia |
| 6 | Ciencia |
| 7 | Autoayuda |

## Motivos de recomendación

Cada libro recomendado incluye una razón explícita, por ejemplo:

- `Muy prestado tras tu libro y genero favorito`
- `Frecuentemente prestado despues de tu libro`
- `Altamente calificado en tu genero favorito`
- `Alta calificacion entre lectores similares`
- `Popular en la biblioteca`
