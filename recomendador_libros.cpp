// ============================================================
//  Recomendador de Libros para Biblioteca
//  Estructura: Grafo dirigido y ponderado (lista de adyacencia)
//  Algoritmo : BFS ponderado con filtros de preferencias
//  Compatible: C++17 — OnlineGDB / g++
// ============================================================

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <set>
#include <limits>

using namespace std;

// ─────────────────────────────────────────────
//  ESTRUCTURAS DE DATOS
// ─────────────────────────────────────────────

struct Libro {
    int    id;
    string titulo;
    string autor;
    string genero;
    int    anio;
    float  calificacion;
    int    veces_prestado;
};

struct Preferencias {
    vector<string> generos;      // géneros favoritos del usuario
    int   anio_min;              // año mínimo de publicación
    int   anio_max;              // año máximo de publicación
    float calificacion_minima;
};

struct Usuario {
    int            id;
    string         nombre;
    vector<int>    historial;    // ids de libros prestados (orden cronológico)
    Preferencias   prefs;
};

// ─────────────────────────────────────────────
//  GRAFO: lista de adyacencia
//  grafo[id_origen] = { {id_destino, peso}, ... }
// ─────────────────────────────────────────────
unordered_map<int, vector<pair<int,int>>> grafo;

// Catálogo de 50 libros
vector<Libro> catalogo;

// Usuarios registrados
vector<Usuario> usuarios;

// ─────────────────────────────────────────────
//  INICIALIZACIÓN DEL CATÁLOGO (50 libros)
// ─────────────────────────────────────────────
void inicializarCatalogo() {
    catalogo = {
        // id, titulo, autor, genero, anio, calif, veces_prestado
        { 1, "Dune",                          "Frank Herbert",         "Ciencia Ficcion", 1965, 4.8f, 120},
        { 2, "Fundacion",                     "Isaac Asimov",          "Ciencia Ficcion", 1951, 4.7f, 115},
        { 3, "Neuromancer",                   "William Gibson",        "Ciencia Ficcion", 1984, 4.5f,  98},
        { 4, "Hyperion",                      "Dan Simmons",           "Ciencia Ficcion", 1989, 4.6f, 105},
        { 5, "El fin de la eternidad",        "Isaac Asimov",          "Ciencia Ficcion", 1955, 4.4f,  87},
        { 6, "Yo Robot",                      "Isaac Asimov",          "Ciencia Ficcion", 1950, 4.5f,  93},
        { 7, "La guerra de los mundos",       "H.G. Wells",            "Ciencia Ficcion", 1898, 4.2f,  80},
        { 8, "Fahrenheit 451",                "Ray Bradbury",          "Ciencia Ficcion", 1953, 4.6f, 110},
        { 9, "Un mundo feliz",                "Aldous Huxley",         "Ciencia Ficcion", 1932, 4.5f, 102},
        {10, "1984",                          "George Orwell",         "Ciencia Ficcion", 1949, 4.7f, 130},
        {11, "El nombre del viento",          "Patrick Rothfuss",      "Fantasia",        2007, 4.7f, 118},
        {12, "El senor de los anillos",       "J.R.R. Tolkien",        "Fantasia",        1954, 4.9f, 145},
        {13, "Harry Potter y la piedra",      "J.K. Rowling",          "Fantasia",        1997, 4.8f, 160},
        {14, "Juego de tronos",               "George R.R. Martin",    "Fantasia",        1996, 4.7f, 140},
        {15, "La historia interminable",      "Michael Ende",          "Fantasia",        1979, 4.6f, 108},
        {16, "El hobbit",                     "J.R.R. Tolkien",        "Fantasia",        1937, 4.8f, 135},
        {17, "Las cronicas de Narnia",        "C.S. Lewis",            "Fantasia",        1950, 4.5f, 112},
        {18, "Eragon",                        "Christopher Paolini",   "Fantasia",        2003, 4.3f,  90},
        {19, "Mistborn",                      "Brandon Sanderson",     "Fantasia",        2006, 4.6f, 100},
        {20, "El color de la magia",          "Terry Pratchett",       "Fantasia",        1983, 4.4f,  85},
        {21, "Crimen y castigo",              "Fiodor Dostoievski",    "Clasico",         1866, 4.6f,  95},
        {22, "Anna Karenina",                 "Leon Tolstoi",          "Clasico",         1878, 4.5f,  88},
        {23, "Don Quijote de la Mancha",      "Miguel de Cervantes",   "Clasico",         1605, 4.7f,  99},
        {24, "Moby Dick",                     "Herman Melville",       "Clasico",         1851, 4.3f,  75},
        {25, "El gran Gatsby",                "F. Scott Fitzgerald",   "Clasico",         1925, 4.4f,  92},
        {26, "Cien anos de soledad",          "Gabriel Garcia Marquez","Clasico",         1967, 4.8f, 125},
        {27, "El retrato de Dorian Gray",     "Oscar Wilde",           "Clasico",         1890, 4.5f,  96},
        {28, "Orgullo y prejuicio",           "Jane Austen",           "Clasico",         1813, 4.6f, 107},
        {29, "La metamorfosis",               "Franz Kafka",           "Clasico",         1915, 4.4f,  89},
        {30, "Los miserables",                "Victor Hugo",           "Clasico",         1862, 4.7f, 111},
        {31, "El codigo Da Vinci",            "Dan Brown",             "Thriller",        2003, 4.2f, 132},
        {32, "Angeles y demonios",            "Dan Brown",             "Thriller",        2000, 4.1f, 120},
        {33, "El silencio de los corderos",   "Thomas Harris",         "Thriller",        1988, 4.5f, 115},
        {34, "Gone Girl",                     "Gillian Flynn",         "Thriller",        2012, 4.3f, 108},
        {35, "La chica del tren",             "Paula Hawkins",         "Thriller",        2015, 4.2f, 103},
        {36, "Inferno",                       "Dan Brown",             "Thriller",        2013, 4.0f,  97},
        {37, "El origen",                     "Dan Brown",             "Thriller",        2017, 4.1f,  94},
        {38, "La sombra del viento",          "Carlos Ruiz Zafon",     "Thriller",        2001, 4.6f, 116},
        {39, "El juego de Ripper",            "Isabel Allende",        "Thriller",        2014, 4.2f,  88},
        {40, "Shutter Island",                "Dennis Lehane",         "Thriller",        2003, 4.3f,  91},
        {41, "Sapiens",                       "Yuval Noah Harari",     "Historia",        2011, 4.7f, 128},
        {42, "Homo Deus",                     "Yuval Noah Harari",     "Historia",        2015, 4.5f, 110},
        {43, "El mundo de ayer",              "Stefan Zweig",          "Historia",        1942, 4.6f,  97},
        {44, "Breve historia del tiempo",     "Stephen Hawking",       "Ciencia",         1988, 4.6f, 118},
        {45, "El gen egoista",                "Richard Dawkins",       "Ciencia",         1976, 4.5f, 105},
        {46, "Una breve historia de casi todo","Bill Bryson",          "Ciencia",         2003, 4.6f, 112},
        {47, "El poder del ahora",            "Eckhart Tolle",         "Autoayuda",       1997, 4.3f,  95},
        {48, "Habitos atomicos",              "James Clear",           "Autoayuda",       2018, 4.7f, 138},
        {49, "Piense y hagase rico",          "Napoleon Hill",         "Autoayuda",       1937, 4.2f,  90},
        {50, "El alquimista",                 "Paulo Coelho",          "Autoayuda",       1988, 4.5f, 122}
    };
}

// ─────────────────────────────────────────────
//  INICIALIZACIÓN DEL GRAFO
//  Relaciones de copréstamo simuladas
// ─────────────────────────────────────────────
void agregarArista(int origen, int destino, int peso) {
    // Si ya existe la arista, acumula el peso
    auto& vecinos = grafo[origen];
    for (auto& par : vecinos) {
        if (par.first == destino) { par.second += peso; return; }
    }
    vecinos.push_back({destino, peso});
}

void inicializarGrafo() {
    // Ciencia Ficción (1-10)
    agregarArista(1, 2, 45); agregarArista(1, 4, 38); agregarArista(1, 3, 30);
    agregarArista(1, 8, 22); agregarArista(1,10, 18);
    agregarArista(2, 5, 40); agregarArista(2, 6, 35); agregarArista(2, 4, 28);
    agregarArista(2, 1, 30); agregarArista(2,10, 20);
    agregarArista(3, 4, 32); agregarArista(3, 1, 25); agregarArista(3, 8, 18);
    agregarArista(4, 2, 36); agregarArista(4, 3, 29); agregarArista(4, 1, 24);
    agregarArista(5, 2, 38); agregarArista(5, 6, 30); agregarArista(5, 9, 22);
    agregarArista(6, 2, 42); agregarArista(6, 5, 33); agregarArista(6, 7, 18);
    agregarArista(7, 9, 28); agregarArista(7, 8, 24); agregarArista(7,10, 20);
    agregarArista(8, 9, 35); agregarArista(8,10, 30); agregarArista(8, 7, 22);
    agregarArista(9,10, 40); agregarArista(9, 8, 28); agregarArista(9, 7, 20);
    agregarArista(10, 9,38); agregarArista(10, 8,30); agregarArista(10, 1,20);

    // Fantasía (11-20)
    agregarArista(11,12, 48); agregarArista(11,14, 40); agregarArista(11,19, 32);
    agregarArista(12,16, 55); agregarArista(12,11, 42); agregarArista(12,13, 35);
    agregarArista(13,14, 50); agregarArista(13,12, 44); agregarArista(13,17, 30);
    agregarArista(14,11, 45); agregarArista(14,19, 38); agregarArista(14,20, 25);
    agregarArista(15,12, 32); agregarArista(15,17, 28); agregarArista(15,16, 24);
    agregarArista(16,12, 52); agregarArista(16,15, 35); agregarArista(16,17, 28);
    agregarArista(17,16, 30); agregarArista(17,15, 26); agregarArista(17,13, 22);
    agregarArista(18,11, 28); agregarArista(18,19, 35); agregarArista(18,12, 20);
    agregarArista(19,11, 40); agregarArista(19,18, 32); agregarArista(19,14, 28);
    agregarArista(20,11, 30); agregarArista(20,19, 25); agregarArista(20,15, 20);

    // Clásicos (21-30)
    agregarArista(21,22, 38); agregarArista(21,29, 30); agregarArista(21,30, 25);
    agregarArista(22,21, 36); agregarArista(22,28, 32); agregarArista(22,30, 22);
    agregarArista(23,26, 42); agregarArista(23,30, 35); agregarArista(23,21, 20);
    agregarArista(24,25, 28); agregarArista(24,27, 24); agregarArista(24,23, 18);
    agregarArista(25,27, 35); agregarArista(25,28, 30); agregarArista(25,24, 22);
    agregarArista(26,23, 40); agregarArista(26,30, 35); agregarArista(26,50, 28);
    agregarArista(27,25, 32); agregarArista(27,28, 28); agregarArista(27,29, 22);
    agregarArista(28,22, 38); agregarArista(28,27, 30); agregarArista(28,25, 25);
    agregarArista(29,21, 32); agregarArista(29,27, 28); agregarArista(29,30, 20);
    agregarArista(30,21, 35); agregarArista(30,26, 30); agregarArista(30,23, 25);

    // Thriller (31-40)
    agregarArista(31,32, 55); agregarArista(31,36, 48); agregarArista(31,37, 42);
    agregarArista(32,31, 50); agregarArista(32,36, 40); agregarArista(32,33, 28);
    agregarArista(33,34, 38); agregarArista(33,40, 32); agregarArista(33,35, 25);
    agregarArista(34,35, 40); agregarArista(34,33, 32); agregarArista(34,40, 28);
    agregarArista(35,34, 38); agregarArista(35,33, 30); agregarArista(35,39, 22);
    agregarArista(36,31, 45); agregarArista(36,37, 42); agregarArista(36,32, 35);
    agregarArista(37,36, 40); agregarArista(37,31, 35); agregarArista(37,38, 25);
    agregarArista(38,31, 30); agregarArista(38,39, 28); agregarArista(38,40, 22);
    agregarArista(39,38, 32); agregarArista(39,35, 28); agregarArista(39,34, 22);
    agregarArista(40,33, 35); agregarArista(40,34, 30); agregarArista(40,38, 25);

    // Historia / Ciencia / Autoayuda (41-50) con puentes entre géneros
    agregarArista(41,42, 50); agregarArista(41,43, 35); agregarArista(41,44, 28);
    agregarArista(42,41, 48); agregarArista(42,43, 30); agregarArista(42,45, 22);
    agregarArista(43,41, 32); agregarArista(43,30, 20); agregarArista(43,21, 18);
    agregarArista(44,45, 45); agregarArista(44,46, 40); agregarArista(44,41, 25);
    agregarArista(45,44, 42); agregarArista(45,46, 38); agregarArista(45,41, 20);
    agregarArista(46,44, 40); agregarArista(46,45, 35); agregarArista(46,41, 22);
    agregarArista(47,48, 45); agregarArista(47,50, 38); agregarArista(47,49, 30);
    agregarArista(48,47, 42); agregarArista(48,49, 35); agregarArista(48,50, 28);
    agregarArista(49,48, 38); agregarArista(49,47, 32); agregarArista(49,50, 28);
    agregarArista(50,48, 40); agregarArista(50,47, 35); agregarArista(50,26, 22);

    // Puentes entre géneros (amplifican sugerencias cruzadas)
    agregarArista(10, 41, 15); agregarArista(41, 10, 15);
    agregarArista( 8, 47, 12); agregarArista(47,  8, 12);
    agregarArista(26, 50, 20); agregarArista(50, 26, 20);
    agregarArista( 2, 44, 18); agregarArista(44,  2, 18);
    agregarArista(12, 23, 14); agregarArista(23, 12, 14);
    agregarArista(38, 26, 16); agregarArista(26, 38, 16);
}

// ─────────────────────────────────────────────
//  UTILIDADES
// ─────────────────────────────────────────────

Libro* buscarLibroPorId(int id) {
    for (auto& l : catalogo)
        if (l.id == id) return &l;
    return nullptr;
}

Libro* buscarLibroPorTitulo(const string& titulo) {
    for (auto& l : catalogo) {
        string t = l.titulo;
        // comparación case-insensitive simple
        if (t == titulo) return &l;
    }
    return nullptr;
}

Usuario* buscarUsuario(int id) {
    for (auto& u : usuarios)
        if (u.id == id) return &u;
    return nullptr;
}

bool generoEnPreferencias(const string& genero, const Preferencias& prefs) {
    for (const auto& g : prefs.generos)
        if (g == genero) return true;
    return false;
}

void limpiarBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pausar() {
    cout << "\nPresione Enter para continuar...";
    limpiarBuffer();
    cin.get();
}

void imprimirSeparador() {
    cout << string(60, '=') << "\n";
}

void imprimirLibro(const Libro& l, int posicion = -1) {
    if (posicion > 0) cout << posicion << ". ";
    cout << left << setw(38) << l.titulo
         << " | " << setw(17) << l.autor
         << " | " << setw(15) << l.genero
         << " | " << l.anio
         << " | ★ " << fixed << setprecision(1) << l.calificacion
         << "\n";
}

// ─────────────────────────────────────────────
//  ALGORITMO DE RECOMENDACIÓN (BFS ponderado)
//  Retorna pares {id_libro, motivo}
// ─────────────────────────────────────────────

// Determina si existe arista directa desde el semilla hacia un libro
bool esVecinoDirecto(int idSemilla, int idCandidato) {
    if (grafo.find(idSemilla) == grafo.end()) return false;
    for (int vi = 0; vi < (int)grafo[idSemilla].size(); vi++)
        if (grafo[idSemilla][vi].first == idCandidato) return true;
    return false;
}

// Obtiene el peso de la arista directa (0 si no existe)
int pesoDirecto(int idSemilla, int idCandidato) {
    if (grafo.find(idSemilla) == grafo.end()) return 0;
    for (int vi = 0; vi < (int)grafo[idSemilla].size(); vi++)
        if (grafo[idSemilla][vi].first == idCandidato)
            return grafo[idSemilla][vi].second;
    return 0;
}

// Motivos posibles (pueden combinarse con '|')
// "Prestado frecuentemente tras tu libro"
// "Genero favorito"
// "Alta calificacion en la biblioteca"
// "Popular en la biblioteca"
string determinarMotivo(int idSemilla, int idCandidato,
                        const Usuario& usuario, bool esRelleno) {
    Libro* l = buscarLibroPorId(idCandidato);
    if (!l) return "";

    bool directo   = esVecinoDirecto(idSemilla, idCandidato);
    bool favorito  = generoEnPreferencias(l->genero, usuario.prefs);
    bool altaCalif = l->calificacion >= 4.5f;

    if (esRelleno) {
        // Llegó aquí porque no había suficientes candidatos en el grafo
        if (favorito)  return "Popular en tu genero favorito";
        return "Muy popular en la biblioteca";
    }

    // Caso 1: conexión directa + género favorito
    if (directo && favorito)
        return "Muy prestado tras tu libro y genero favorito";

    // Caso 2: conexión directa sin coincidencia de género
    if (directo && !favorito) {
        int peso = pesoDirecto(idSemilla, idCandidato);
        if (peso >= 30) return "Muy frecuentemente prestado despues de tu libro";
        return "Frecuentemente prestado despues de tu libro";
    }

    // Caso 3: sin conexión directa (llegó por profundidad 2 o 3) + género favorito
    if (!directo && favorito && altaCalif)
        return "Altamente calificado en tu genero favorito";
    if (!directo && favorito)
        return "Coincide con tu genero favorito";

    // Caso 4: sin conexión directa, sin género favorito
    if (altaCalif) return "Alta calificacion entre lectores similares";
    return "Recomendado por lectores con gustos similares";
}

vector<pair<int,string>> recomendar(int idSemilla, const Usuario& usuario, int topN = 5) {
    // Score acumulado por id de libro
    map<int, float> scores;

    // Set de libros ya prestados por el usuario (excluir de recomendaciones)
    set<int> yaPrestados(usuario.historial.begin(), usuario.historial.end());

    // BFS: {id_libro, profundidad}
    queue<pair<int,int>> bfsQueue;
    set<int> visitados;

    bfsQueue.push(make_pair(idSemilla, 0));
    visitados.insert(idSemilla);

    const int MAX_PROFUNDIDAD = 3;

    while (!bfsQueue.empty()) {
        int idActual = bfsQueue.front().first;
        int prof     = bfsQueue.front().second;
        bfsQueue.pop();

        if (prof >= MAX_PROFUNDIDAD) continue;

        float decaimiento = 1.0f / (prof + 1);

        if (grafo.find(idActual) == grafo.end()) continue;

        for (int vi = 0; vi < (int)grafo[idActual].size(); vi++) {
            int idVecino = grafo[idActual][vi].first;
            int peso     = grafo[idActual][vi].second;
            Libro* vecino = buscarLibroPorId(idVecino);
            if (!vecino) continue;

            if (vecino->calificacion < usuario.prefs.calificacion_minima) continue;
            if (vecino->anio < usuario.prefs.anio_min) continue;
            if (vecino->anio > usuario.prefs.anio_max) continue;

            float factorGenero = generoEnPreferencias(vecino->genero, usuario.prefs) ? 1.5f : 1.0f;
            float scoreIncremento = peso * vecino->calificacion * factorGenero * decaimiento;

            scores[idVecino] += scoreIncremento;

            if (visitados.find(idVecino) == visitados.end()) {
                visitados.insert(idVecino);
                bfsQueue.push(make_pair(idVecino, prof + 1));
            }
        }
    }

    scores.erase(idSemilla);
    for (int id : yaPrestados) scores.erase(id);

    vector<pair<float,int>> ranking;
    for (map<int,float>::iterator it = scores.begin(); it != scores.end(); ++it)
        ranking.push_back(make_pair(it->second, it->first));
    sort(ranking.begin(), ranking.end(), greater<pair<float,int>>());

    vector<pair<int,string>> resultado;
    for (int i = 0; i < (int)ranking.size() && (int)resultado.size() < topN; i++) {
        int idCand = ranking[i].second;
        string motivo = determinarMotivo(idSemilla, idCand, usuario, false);
        resultado.push_back(make_pair(idCand, motivo));
    }

    // Completar con libros populares si faltan
    if ((int)resultado.size() < topN) {
        vector<Libro> populares = catalogo;
        sort(populares.begin(), populares.end(),
             [](const Libro& a, const Libro& b){ return a.veces_prestado > b.veces_prestado; });
        for (int pi = 0; pi < (int)populares.size(); pi++) {
            if ((int)resultado.size() >= topN) break;
            int lid = populares[pi].id;
            if (yaPrestados.count(lid)) continue;
            if (lid == idSemilla) continue;
            bool yaEsta = false;
            for (int ri = 0; ri < (int)resultado.size(); ri++)
                if (resultado[ri].first == lid) { yaEsta = true; break; }
            if (yaEsta) continue;
            string motivo = determinarMotivo(idSemilla, lid, usuario, true);
            resultado.push_back(make_pair(lid, motivo));
        }
    }

    return resultado;
}

// ─────────────────────────────────────────────
//  REGISTRO DE PRÉSTAMO (actualiza grafo)
// ─────────────────────────────────────────────
void registrarPrestamo(Usuario& u, int idLibro) {
    // Si el usuario ya tiene historial, crear arista entre último y nuevo
    if (!u.historial.empty()) {
        int ultimo = u.historial.back();
        agregarArista(ultimo, idLibro, 1);
    }
    u.historial.push_back(idLibro);

    // Incrementar contador global del libro
    Libro* l = buscarLibroPorId(idLibro);
    if (l) l->veces_prestado++;
}

// ─────────────────────────────────────────────
//  MENÚ: REGISTRAR USUARIO
// ─────────────────────────────────────────────
void menuRegistrarUsuario() {
    imprimirSeparador();
    cout << "  REGISTRAR NUEVO USUARIO\n";
    imprimirSeparador();

    Usuario u;
    u.id = (int)usuarios.size() + 1;

    limpiarBuffer();
    cout << "Nombre: ";
    getline(cin, u.nombre);

    // Preferencias de género
    cout << "\nGeneros disponibles:\n";
    cout << "  1. Ciencia Ficcion\n";
    cout << "  2. Fantasia\n";
    cout << "  3. Clasico\n";
    cout << "  4. Thriller\n";
    cout << "  5. Historia\n";
    cout << "  6. Ciencia\n";
    cout << "  7. Autoayuda\n";
    cout << "Ingrese numeros de generos favoritos separados por espacios (ej: 1 2): ";

    string linea;
    getline(cin, linea);
    vector<string> nombresGeneros = {
        "Ciencia Ficcion","Fantasia","Clasico",
        "Thriller","Historia","Ciencia","Autoayuda"
    };
    for (char c : linea) {
        int n = c - '0';
        if (n >= 1 && n <= 7)
            u.prefs.generos.push_back(nombresGeneros[n-1]);
    }
    if (u.prefs.generos.empty()) u.prefs.generos.push_back("Ciencia Ficcion");

    cout << "Anio minimo de publicacion (ej: 1950): ";
    cin >> u.prefs.anio_min;
    cout << "Anio maximo de publicacion (ej: 2024): ";
    cin >> u.prefs.anio_max;
    cout << "Calificacion minima aceptable (1.0 - 5.0): ";
    cin >> u.prefs.calificacion_minima;

    usuarios.push_back(u);
    cout << "\n✓ Usuario registrado con ID: " << u.id << "\n";
    pausar();
}

// ─────────────────────────────────────────────
//  MENÚ: REGISTRAR PRÉSTAMO
// ─────────────────────────────────────────────
void menuRegistrarPrestamo(Usuario& u) {
    imprimirSeparador();
    cout << "  REGISTRAR PRESTAMO — Usuario: " << u.nombre << "\n";
    imprimirSeparador();
    cout << "Ingrese el ID del libro (1-50): ";
    int id; cin >> id;
    Libro* l = buscarLibroPorId(id);
    if (!l) { cout << "ID de libro no encontrado.\n"; pausar(); return; }

    registrarPrestamo(u, id);
    cout << "✓ Prestamo registrado: \"" << l->titulo << "\"\n";
    pausar();
}

// ─────────────────────────────────────────────
//  MENÚ: VER HISTORIAL
// ─────────────────────────────────────────────
void menuVerHistorial(const Usuario& u) {
    imprimirSeparador();
    cout << "  HISTORIAL DE PRESTAMOS — " << u.nombre << "\n";
    imprimirSeparador();
    if (u.historial.empty()) { cout << "Sin historial de prestamos.\n"; pausar(); return; }
    for (int i = 0; i < (int)u.historial.size(); i++) {
        Libro* l = buscarLibroPorId(u.historial[i]);
        if (l) {
            cout << i+1 << ". " << l->titulo << " (" << l->genero << ", " << l->anio << ")\n";
        }
    }
    pausar();
}

// ─────────────────────────────────────────────
//  MENÚ: VER CATÁLOGO
// ─────────────────────────────────────────────
void menuVerCatalogo() {
    imprimirSeparador();
    cout << "  CATALOGO DE LIBROS\n";
    imprimirSeparador();
    cout << left << setw(4) << "ID"
         << setw(38) << "Titulo"
         << setw(20) << "Autor"
         << setw(16) << "Genero"
         << setw(6)  << "Anio"
         << "Calif.\n";
    cout << string(90, '-') << "\n";
    for (auto& l : catalogo) {
        cout << setw(4) << l.id;
        imprimirLibro(l);
    }
    pausar();
}

// ─────────────────────────────────────────────
//  MENÚ: OBTENER RECOMENDACIONES
// ─────────────────────────────────────────────
void menuRecomendaciones(const Usuario& u) {
    imprimirSeparador();
    cout << "  RECOMENDACIONES — " << u.nombre << "\n";
    imprimirSeparador();

    int idSemilla = -1;

    if (!u.historial.empty()) {
        idSemilla = u.historial.back();
        Libro* semilla = buscarLibroPorId(idSemilla);
        cout << "Libro de referencia (ultimo prestado): \""
             << semilla->titulo << "\" [" << semilla->genero << "]\n\n";
        cout << "1. Usar este libro como referencia\n";
        cout << "2. Ingresar otro libro de referencia\n";
        cout << "Opcion: ";
        int op; cin >> op;
        if (op == 2) {
            cout << "ID del libro de referencia: ";
            cin >> idSemilla;
        }
    } else {
        cout << "No tiene historial. Ingrese el ID de un libro de referencia: ";
        cin >> idSemilla;
    }

    Libro* semilla = buscarLibroPorId(idSemilla);
    if (!semilla) { cout << "ID no valido.\n"; pausar(); return; }

    vector<pair<int,string>> recomendados = recomendar(idSemilla, u, 5);

    cout << "\n";
    imprimirSeparador();
    cout << "  TOP 5 RECOMENDACIONES PARA: " << u.nombre << "\n";
    cout << "  Basadas en: \"" << semilla->titulo << "\"\n";
    imprimirSeparador();

    if (recomendados.empty()) {
        cout << "No se encontraron recomendaciones con sus preferencias actuales.\n";
    } else {
        for (int i = 0; i < (int)recomendados.size(); i++) {
            Libro* l = buscarLibroPorId(recomendados[i].first);
            if (!l) continue;
            string motivo = recomendados[i].second;
            cout << i+1 << ". " << left << setw(34) << l->titulo
                 << " | " << setw(18) << l->autor
                 << " | " << setw(15) << l->genero
                 << " | ★ " << fixed << setprecision(1) << l->calificacion
                 << "\n"
                 << "   >> " << motivo << "\n";
        }
    }
    pausar();
}

// ─────────────────────────────────────────────
//  MENÚ: CALIFICAR LIBRO
// ─────────────────────────────────────────────
void menuCalificarLibro(const Usuario& u) {
    imprimirSeparador();
    cout << "  CALIFICAR LIBRO\n";
    imprimirSeparador();
    if (u.historial.empty()) { cout << "No ha prestado libros aun.\n"; pausar(); return; }
    cout << "Sus libros prestados:\n";
    for (int id : u.historial) {
        Libro* l = buscarLibroPorId(id);
        if (l) cout << "  [" << id << "] " << l->titulo << "\n";
    }
    cout << "ID del libro a calificar: ";
    int id; cin >> id;
    Libro* l = buscarLibroPorId(id);
    if (!l) { cout << "ID no encontrado.\n"; pausar(); return; }
    cout << "Calificacion (1.0 a 5.0): ";
    float nueva; cin >> nueva;
    if (nueva < 1.0f) nueva = 1.0f;
    if (nueva > 5.0f) nueva = 5.0f;
    // Promedio simple con la calificación existente
    l->calificacion = (l->calificacion + nueva) / 2.0f;
    cout << "✓ Nueva calificacion promedio de \"" << l->titulo
         << "\": " << fixed << setprecision(1) << l->calificacion << "\n";
    pausar();
}

// ─────────────────────────────────────────────
//  MENÚ PRINCIPAL
// ─────────────────────────────────────────────
int main() {

    inicializarCatalogo();
    inicializarGrafo();

    // Usuario de demostración precargado
    Usuario demo;
    demo.id = 1;
    demo.nombre = "Demo";
    demo.prefs.generos = {"Ciencia Ficcion", "Fantasia"};
    demo.prefs.anio_min = 1940;
    demo.prefs.anio_max = 2024;
    demo.prefs.calificacion_minima = 4.0f;
    demo.historial = {1}; // ya leyó Dune
    catalogo[0].veces_prestado++;
    usuarios.push_back(demo);

    int opcion = -1;
    while (opcion != 0) {
        imprimirSeparador();
        cout << "   SISTEMA RECOMENDADOR DE BIBLIOTECA\n";
        imprimirSeparador();
        cout << "  1. Ver catalogo de libros\n";
        cout << "  2. Registrar nuevo usuario\n";
        cout << "  3. Seleccionar usuario activo\n";
        cout << "  4. Registrar prestamo\n";
        cout << "  5. Obtener recomendaciones\n";
        cout << "  6. Ver historial de prestamos\n";
        cout << "  7. Calificar un libro\n";
        cout << "  0. Salir\n";
        imprimirSeparador();

        // Mostrar usuario activo
        static int idActivo = 1;
        Usuario* activo = buscarUsuario(idActivo);
        if (activo)
            cout << "  Usuario activo: " << activo->nombre
                 << " (ID " << idActivo << ")\n";
        cout << "  Opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                menuVerCatalogo();
                break;
            case 2:
                menuRegistrarUsuario();
                break;
            case 3: {
                cout << "Usuarios registrados:\n";
                for (auto& u : usuarios)
                    cout << "  [" << u.id << "] " << u.nombre << "\n";
                cout << "ID del usuario a seleccionar: ";
                cin >> idActivo;
                if (!buscarUsuario(idActivo)) {
                    cout << "ID no encontrado. Se mantiene usuario actual.\n";
                    idActivo = activo ? activo->id : 1;
                }
                pausar();
                break;
            }
            case 4:
                if (activo) menuRegistrarPrestamo(*activo);
                else { cout << "Seleccione un usuario primero.\n"; pausar(); }
                break;
            case 5:
                if (activo) menuRecomendaciones(*activo);
                else { cout << "Seleccione un usuario primero.\n"; pausar(); }
                break;
            case 6:
                if (activo) menuVerHistorial(*activo);
                else { cout << "Seleccione un usuario primero.\n"; pausar(); }
                break;
            case 7:
                if (activo) menuCalificarLibro(*activo);
                else { cout << "Seleccione un usuario primero.\n"; pausar(); }
                break;
            case 0:
                cout << "\n¡Hasta luego!\n";
                break;
            default:
                cout << "Opcion no valida.\n"; pausar();
        }
    }

    return 0;
}
