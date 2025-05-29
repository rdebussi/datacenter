# Compilador e flags
#alter
CXX = g++
CXXFLAGS = -I.

# Diretórios
SRCDIR = src
CONVERTERDIR = $(SRCDIR)/converter
INSPECTDIR = $(SRCDIR)/inspect

# Alvos principais
.PHONY: all clean

all: build_btree query_library achievement_query datacenter test_hash

# Build BTree
build_btree: build_btree.exe

build_btree.exe: $(SRCDIR)/build_btree.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

# Query Library
query_library: query_library.exe

query_library.exe: $(SRCDIR)/query_library.cpp
	$(CXX) $< -o $@ $(CXXFLAGS)

# Achievement Query
achievement_query: achievement_query.exe

achievement_query.exe: $(SRCDIR)/achievement_query.cpp $(SRCDIR)/achievement_index.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Datacenter (programa principal)
datacenter: datacenter.exe

datacenter.exe: $(SRCDIR)/main.cpp \
                $(SRCDIR)/utils.cpp \
                $(CONVERTERDIR)/converter.cpp \
                $(CONVERTERDIR)/converter_game.cpp \
                $(CONVERTERDIR)/converter_price.cpp \
                $(CONVERTERDIR)/converter_achievement.cpp \
                $(CONVERTERDIR)/purchased_games_converter.cpp \
                $(CONVERTERDIR)/converter_history.cpp \
                $(INSPECTDIR)/inspect_game.cpp \
                $(INSPECTDIR)/inspect_player.cpp \
                $(INSPECTDIR)/inspect_price.cpp \
                $(INSPECTDIR)/inspect_achievement.cpp \
                $(INSPECTDIR)/purchased_games_inspect.cpp \
                $(INSPECTDIR)/inspect_history.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Test Hash
test_hash: test_hash.exe

test_hash.exe: $(SRCDIR)/test_hash.cpp $(SRCDIR)/hash_table.cpp $(SRCDIR)/utils.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)

# Limpeza
clean:
	rm -f *.exe