# =======================================================
# MAKEFILE - Stock Exchange Matching Engine (CLOB)
# Compilación directa sin CMake
# =======================================================

# Directorios y nombres de archivos
BUILD_DIR := build
SRC_FILES := main.cpp orderBook.cpp dashboardImp.cpp
HDR_FILES := Order.h dashboard.h utility.h orderdef.h
OBJ_FILES := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
EXECUTABLE := $(BUILD_DIR)/clob_engine

# Compilador y Banderas
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic
# Incluye el directorio actual (donde están los archivos .h)
INC_FLAGS := -I.

# =======================================================
# OBJETIVOS PRINCIPALES
# =======================================================

.PHONY: all run clean help

# El objetivo por defecto es 'all': construye el ejecutable.
all: $(EXECUTABLE)

# Objetivo 'run': construye (si es necesario) y ejecuta el programa.
run: $(EXECUTABLE)
	@echo "Ejecutando la aplicación. Presiona Ctrl+C para detenerla..."
	@$(EXECUTABLE)

# Objetivo 'clean': elimina los archivos objeto y el ejecutable.
clean:
	@echo "Limpiando archivos de compilación..."
	@rm -rf $(BUILD_DIR)

# =======================================================
# REGLAS DE COMPILACIÓN
# =======================================================

# Regla para crear el directorio de construcción si no existe
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Regla para enlazar: crear el ejecutable a partir de los archivos objeto
$(EXECUTABLE): $(OBJ_FILES) | $(BUILD_DIR)
	@echo "Enlazando..."
	$(CXX) $(OBJ_FILES) -o $@

# Regla para compilar archivos fuente (.cpp) a objetos (.o)
# Depende de los archivos .h para la recompilación.
$(BUILD_DIR)/%.o: %.cpp $(HDR_FILES) | $(BUILD_DIR)
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) $(INC_FLAGS) -c $< -o $@

# Mensaje de ayuda (igual que el anterior para referencia)
help:
	@echo "--------------------------------------------------------"
	@echo "Makefile para Stock Exchange Matching Engine (CLOB)"
	@echo "--------------------------------------------------------"
	@echo "Comandos disponibles:"
	@echo "  make all   : Compila todo el proyecto."
	@echo "  make run   : Compila (si es necesario) y ejecuta el CLOB."
	@echo "  make clean : Elimina el directorio de construcción ('build/')."
	@echo "  make help  : Muestra este mensaje de ayuda."
	@echo "--------------------------------------------------------"
