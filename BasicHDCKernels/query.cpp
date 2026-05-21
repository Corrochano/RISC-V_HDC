/*
Copyright 2026 Álvaro Corrochano López

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "typeDefine.h"

using namespace std;

int main(){
// 1. Configuración de dimensiones
    size_t nvec = 3;          // Vamos a comparar contra 3 vectores en la matriz M
    size_t words = 8;         // 8 words * 8 bytes = 64 bytes (así coincide con la alineación)
    size_t alignment = 64;

    size_t total_bytes = words * sizeof(hdc_word_t);
    size_t alloc_size = ((total_bytes + (alignment - 1)) / alignment) * alignment;

    // 2. Asignación de memoria alineada
    // Matriz M: Contiene 'nvec' vectores seguidos
    size_t total_M_bytes = nvec * words * sizeof(hdc_word_t);
    size_t alloc_size_M = ((total_M_bytes + (alignment - 1)) / alignment) * alignment;
    hdc_word_t *M = (hdc_word_t*)aligned_alloc(alignment, alloc_size_M);

    // Vector de consulta q
    hdc_word_t *q = (hdc_word_t*)aligned_alloc(alignment, alloc_size);

    // Array de puntuaciones (un score por cada vector en M)
    size_t total_scores_bytes = nvec * sizeof(hdc_score_t);
    size_t alloc_size_scores = ((total_scores_bytes + (alignment - 1)) / alignment) * alignment;
    hdc_score_t *scores = (hdc_score_t*)aligned_alloc(alignment, alloc_size_scores);

    // 3. Inicialización de datos de prueba
    // Rellenamos la matriz M con patrones predecibles
    for (size_t i = 0; i < nvec; i++) {
        for (size_t j = 0; j < words; j++) {
            // El vector 0 tendrá valores bajos, el vector 2 valores más altos
            M[i * words + j] = i + j; 
        }
    }

    // Rellenamos el query q
    for (size_t j = 0; j < words; j++) {
        q[j] = j; // Idéntico al primer vector de M (i=0) para que el score sea perfecto/bajo
    }

    // Inicializamos los scores a 0
    for (size_t i = 0; i < nvec; i++) {
        scores[i] = 0;
    }

    /*
    printf("Vectores en la matriz:\n");
    for (size_t i = 0; i < nvec * words; i++) {
        printf("Vector M[%lu]\n", i % words);
        printf("%lu\n", M[i]);
        printf("\n");
    }

    printf("Vector q:\n");
    for(size_t j = 0; j < words; j++){
            printf("%lu\n", q[j]);
    }
    */

    printf("Ejecutando hdc_query para %lu vectores...\n", nvec);
    hdc_query(M, q, scores, nvec, words, alignment, alloc_size);

    // 5. Mostrar resultados
    printf("\n--- RESULTADOS ---\n");
    for (size_t i = 0; i < nvec; i++) {
        printf("Vector M[%lu] -> Score: %lu\n", i, scores[i]);
    }

    // 6. Liberar memoria
    free(M);
    free(q);
    free(scores);

    return 0;
}
