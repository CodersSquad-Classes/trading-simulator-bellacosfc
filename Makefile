# ===============================================
# Configuración del Proyecto
# ===============================================

# Nombre del ejecutable final
TARGET = clob_engine

# Compilador C++
CXX = g++

# Archivos fuente (.cpp)
SOURCES = \
	main.cpp \
	OrderBook.cpp \
	Dashboard.cpp

# Archivos objeto (.o) - generados automáticamente a partir de SOURCES
OBJECTS = $(SOURCES:.cpp=.o)

# ===============================================
# Banderas y Opciones de Compilación
# ===============================================

# Estándar C++17 (-std=c++17)
# Advertencias de compilación (-Wall -Wextra -Wpedantic)
# Incluir directorio actual para headers (-I.)
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -I.

# Banderas del linker: Necesarias para enlazar la biblioteca de hilos (Threads::Threads)
LDFLAGS = -pthread

# ===============================================
# Reglas del Makefile
# ===============================================

# 1. Regla por defecto: Construir el ejecutable
.PHONY: all
all: $(TARGET)

# Enlazar los archivos objeto para crear el ejecutable
$(TARGET): $(OBJECTS)
	@echo "Enlazando $^ para crear $@..."
	$(CXX) $^ -o $@ $(LDFLAGS)

# 2. Regla de Compilación (Regla de sufijo genérica)
# Compila cualquier archivo .cpp a su archivo .o correspondiente.
# $< es la dependencia (el archivo .cpp).
# $@ es el destino (el archivo .o).
%.o: %.cpp
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 3. Regla de Limpieza
# Elimina los archivos objeto (.o) y el ejecutable
.PHONY: clean
clean:
	@echo "Limpiando archivos de compilación..."
	rm -f $(TARGET) $(OBJECTS)
