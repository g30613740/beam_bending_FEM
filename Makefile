PROJECT_ROOT := $(shell pwd)
SRC_DIR      := $(PROJECT_ROOT)/src
SAGE_DIR     := $(SRC_DIR)/sage
FEM_DIR      := $(SRC_DIR)/fem
PLOT_DIR     := $(SRC_DIR)/plotting
DATA_DIR     := $(PROJECT_ROOT)/data
RESULTS_DIR  := $(PROJECT_ROOT)/results

# Компилятор и флаги для C++
CXX      := g++
CXXFLAGS := -std=c++11 -Wall -O2
FEM_TARGET := $(PROJECT_ROOT)/beams_bending

# Python и Sage
PYTHON   := python3
SAGE     := sage

# Файлы данных (симлинки создаются в корне, если их нет)
PARAMS_DATA := $(DATA_DIR)/parameters.txt
COORDS_DATA := $(DATA_DIR)/coordinates.txt
PARAMS_LINK := $(PROJECT_ROOT)/parameters.txt
COORDS_LINK := $(PROJECT_ROOT)/coordinates.txt

# Выходные файлы (результаты)
SAGE_RESULTS := $(wildcard $(RESULTS_DIR)/sage_res_*.txt)
FEM_RESULTS  := $(wildcard $(RESULTS_DIR)/FEM_res_*.txt)
PLOT_OUTPUT  := $(RESULTS_DIR)/result_plot.png

# Цель по умолчанию: всё (расчёт + графики)
.PHONY: all
all: sage fem plot
	@echo "Все расчёты выполнены. Графики построены."

# -------------------------------
# Аналитическое решение (Sage)
# -------------------------------
.PHONY: sage
sage: check_data_links
	@echo " Запуск аналитического решения в Sage..."
	cd $(SAGE_DIR) && $(SAGE) beams_bending.sage
	@echo " Результаты Sage сохранены в $(RESULTS_DIR)"

# -------------------------------
# Численное решение (C++ FEM)
# -------------------------------

# Цель fem: сначала компиляция (при необходимости), затем запуск
.PHONY: fem
fem: $(FEM_TARGET)
	@echo "Запуск МКЭ (C++)..."
	cd $(FEM_DIR) && ./beams_bending
	@echo "Результаты FEM сохранены в $(RESULTS_DIR)"

# Правило компиляции: создаём исполняемый файл в src/fem
$(FEM_TARGET): $(FEM_DIR)/beams_bending.cpp
	@echo "Компиляция FEM-программы..."
	$(CXX) $(CXXFLAGS) $< -o $@

# -------------------------------
# Построение графиков
# -------------------------------
.PHONY: plot
plot: check_data_links sage fem
	@echo "Построение графиков..."
	cd $(PLOT_DIR) && $(PYTHON) paint.py
	@echo "График сохранён: $(PLOT_OUTPUT)"

# -------------------------------
# Вспомогательные цели
# -------------------------------
.PHONY: check_data_links
check_data_links:
	@if [ ! -f $(PARAMS_LINK) ]; then \
		ln -s $(PARAMS_DATA) $(PARAMS_LINK); \
		echo "Создана символическая ссылка $(PARAMS_LINK) -> $(PARAMS_DATA)"; \
	fi
	@if [ ! -f $(COORDS_LINK) ]; then \
		ln -s $(COORDS_DATA) $(COORDS_LINK); \
		echo "Создана символическая ссылка $(COORDS_LINK) -> $(COORDS_DATA)"; \
	fi

# -------------------------------
# Очистка результатов и скомпилированных файлов
# -------------------------------
.PHONY: clean
clean:
	@echo "Удаление результатов и временных файлов..."
	rm -f $(RESULTS_DIR)/sage_res_*.txt
	rm -f $(RESULTS_DIR)/FEM_res_*.txt
	rm -f $(RESULTS_DIR)/result_plot.png
	rm -f $(FEM_TARGET)
	@echo "Очистка завершена."

# -------------------------------
# Полная пересборка
# -------------------------------
.PHONY: rebuild
rebuild: clean all

# -------------------------------
# Показать справку
# -------------------------------
.PHONY: help
help:
	@echo "Доступные цели:"
	@echo "  all      : выполнить Sage, FEM и построить графики (цель по умолчанию)"
	@echo "  sage     : только аналитическое решение в Sage"
	@echo "  fem      : скомпилировать и запустить FEM (C++)"
	@echo "  plot     : построить графики (запускает sage и fem, если нужно)"
	@echo "  clean    : удалить все результаты, исполняемые файлы и временные файлы"
	@echo "  rebuild  : clean + all"
	@echo "  help     : показать эту справку"