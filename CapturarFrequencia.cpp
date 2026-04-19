#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <array>
#include <cctype>
#include <string>
#include <chrono>
#include <sstream>

struct Candidato {
    std::string texto;
    int score;
};

int contarFrequencia(const std::string& texto, std::unordered_map<char, int>& hash) {
    for (char c : texto) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            char minusculo = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            hash[minusculo]++;
        }
    }
    return 0;
}

std::string normalizarParaBusca(const std::string& texto) {
    std::string normalizado;
    normalizado.reserve(texto.size());

    for (char c : texto) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            normalizado += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        } else {
            normalizado += ' ';
        }
    }

    return normalizado;
}

std::string aplicarMapeamento(const std::string& textoCifrado, const std::array<char, 26>& mapa) {
    std::string textoDecifrado;
    textoDecifrado.reserve(textoCifrado.size());

    for (char c : textoCifrado) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            bool maiuscula = std::isupper(static_cast<unsigned char>(c));
            char idx = static_cast<char>(std::tolower(static_cast<unsigned char>(c)) - 'a');
            char decifrada = mapa[static_cast<size_t>(idx)];
            textoDecifrado += maiuscula
                ? static_cast<char>(std::toupper(static_cast<unsigned char>(decifrada)))
                : decifrada;
        } else {
            textoDecifrado += c;
        }
    }

    return textoDecifrado;
}

int pontuarPortugues(const std::string& texto) {
    const std::unordered_set<std::string> palavrasComuns = {
        "a", "o", "e", "de", "do", "da", "que", "em", "um", "uma",
        "com", "para", "por", "na", "no", "os", "as", "ao", "dos", "das",
        "mais", "se", "como", "mas", "foi", "texto", "codigo", "qualidade", "revisao", "pratica"
    };
    const std::vector<std::string> paresComuns = {
        "de", "da", "do", "qu", "es", "en", "co", "re", "ss", "rr", "ch", "nh", "lh"
    };
    const std::vector<std::string> paresRaros = {
        "kk", "ww", "yy", "wq", "kq", "jq", "qx"
    };

    std::string normalizado = normalizarParaBusca(texto);
    std::istringstream iss(normalizado);
    std::string palavra;
    int score = 0;

    while (iss >> palavra) {
        if (palavrasComuns.find(palavra) != palavrasComuns.end()) {
            score += 12;
        }
    }

    for (const std::string& par : paresComuns) {
        size_t pos = normalizado.find(par);
        while (pos != std::string::npos) {
            score += 2;
            pos = normalizado.find(par, pos + 1);
        }
    }

    for (const std::string& par : paresRaros) {
        size_t pos = normalizado.find(par);
        while (pos != std::string::npos) {
            score -= 5;
            pos = normalizado.find(par, pos + 1);
        }
    }

    return score;
}

std::vector<char> ordenarLetrasPorFrequencia(const std::unordered_map<char, int>& hash) {
    std::vector<std::pair<char, int>> frequencias(hash.begin(), hash.end());
    std::sort(
        frequencias.begin(),
        frequencias.end(),
        [](const std::pair<char, int>& a, const std::pair<char, int>& b) {
            return a.second > b.second;
        }
    );

    std::vector<char> letras;
    letras.reserve(frequencias.size());
    for (const auto& item : frequencias) {
        letras.push_back(item.first);
    }
    return letras;
}

std::vector<Candidato> atacarSubstituicaoPorFrequencia(const std::string& textoCifrado, int quantidadeResultados) {
    const std::vector<char> frequenciaPortugues = {
        'a', 'e', 'o', 's', 'r', 'i', 'n', 'd', 'm', 'u', 't', 'c', 'l', 'p', 'v', 'g', 'h', 'q', 'b', 'f', 'z', 'j', 'x', 'k', 'w', 'y'
    };

    std::unordered_map<char, int> hash;
    contarFrequencia(textoCifrado, hash);
    std::vector<char> letrasCifradasOrdenadas = ordenarLetrasPorFrequencia(hash);

    std::array<char, 26> mapaBase;
    for (int i = 0; i < 26; i++) {
        mapaBase[static_cast<size_t>(i)] = static_cast<char>('a' + i);
    }

    for (size_t i = 0; i < letrasCifradasOrdenadas.size() && i < frequenciaPortugues.size(); i++) {
        size_t idx = static_cast<size_t>(letrasCifradasOrdenadas[i] - 'a');
        mapaBase[idx] = frequenciaPortugues[i];
    }

    std::vector<Candidato> candidatos;
    std::vector<char> prefixo = {
        frequenciaPortugues[0], frequenciaPortugues[1], frequenciaPortugues[2],
        frequenciaPortugues[3], frequenciaPortugues[4], frequenciaPortugues[5]
    };

    std::sort(prefixo.begin(), prefixo.end());
    do {
        std::array<char, 26> mapaAtual = mapaBase;
        for (size_t i = 0; i < letrasCifradasOrdenadas.size() && i < prefixo.size(); i++) {
            size_t idx = static_cast<size_t>(letrasCifradasOrdenadas[i] - 'a');
            mapaAtual[idx] = prefixo[i];
        }

        std::string textoDecifrado = aplicarMapeamento(textoCifrado, mapaAtual);
        int score = pontuarPortugues(textoDecifrado);
        candidatos.push_back({textoDecifrado, score});
    } while (std::next_permutation(prefixo.begin(), prefixo.end()));

    std::sort(
        candidatos.begin(),
        candidatos.end(),
        [](const Candidato& a, const Candidato& b) {
            return a.score > b.score;
        }
    );

    std::vector<Candidato> unicos;
    std::unordered_set<std::string> vistos;
    for (const Candidato& c : candidatos) {
        if (vistos.insert(c.texto).second) {
            unicos.push_back(c);
            if (static_cast<int>(unicos.size()) >= quantidadeResultados) {
                break;
            }
        }
    }

    return unicos;
}


int main() {
    
    // Recebe o texto cifrado
    std::string textoCifrado;
    textoCifrado = "D TXDOLGDGH GR FRGLJR YHP FRP SUDWLFD H UHYLVDR";
    
    int quantidadeResultados = 5;
    std::cout << "Quantidade de hipoteses para exibir: ";
    std::cin >> quantidadeResultados;
    if (quantidadeResultados <= 0) {
        quantidadeResultados = 1;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Candidato> melhores = atacarSubstituicaoPorFrequencia(textoCifrado, quantidadeResultados);

    std::cout << "\nTop " << melhores.size() << " hipoteses:\n";
    for (size_t i = 0; i < melhores.size(); i++) {
        std::cout << "[" << (i + 1) << "] score " << melhores[i].score << " -> "
                  << melhores[i].texto << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "\nTempo gasto: " << elapsed.count() << " segundos" << std::endl;

    return 0;
}