#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_TEXT_LENGTH 1000
#define MAX_UNIQUE_CHARS 128

// Structure to hold encoding information for each character
typedef struct {
    int count;
    int* locations;
    long product;
    int sum;
    int max_location;
    int locations_size;
} EncodingInfo;

// Structure to hold the encoding table
typedef struct {
    char* chars;
    EncodingInfo* info;
    int size;
} EncodingTable;

// Function to multiply all numbers in an array
long multiply_list(int* arr, int size) {
    long product = 1;
    for (int i = 0; i < size; i++) {
        product *= arr[i];
    }
    return product;
}

// Function to find maximum value in an array
int find_max(int* arr, int size) {
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// Function to get all factors of a number
int* get_factors(int n, int* factor_count) {
    int* factors = (int*)malloc(n * sizeof(int));
    *factor_count = 0;
    
    for (int i = 1; i <= n; i++) {
        if (n % i == 0) {
            factors[*factor_count] = i;
            (*factor_count)++;
        }
    }
    return factors;
}

// Function to compress text
EncodingTable compress(const char* plaintext) {
    EncodingTable table;
    table.size = 0;
    table.chars = (char*)malloc(MAX_UNIQUE_CHARS * sizeof(char));
    table.info = (EncodingInfo*)malloc(MAX_UNIQUE_CHARS * sizeof(EncodingInfo));
    
    bool char_exists[128] = {false}; // Track unique characters
    int text_len = strlen(plaintext);
    
    // Find unique characters and count their occurrences
    for (int i = 0; i < text_len; i++) {
        char c = plaintext[i];
        if (!char_exists[c]) {
            char_exists[c] = true;
            table.chars[table.size] = c;
            
            // Count occurrences and store locations
            EncodingInfo* info = &table.info[table.size];
            info->locations = (int*)malloc(text_len * sizeof(int));
            info->locations_size = 0;
            info->count = 0;
            info->sum = 0;
            
            for (int j = 0; j < text_len; j++) {
                if (plaintext[j] == c) {
                    info->locations[info->locations_size] = j + 1; // 1-based indexing
                    info->locations_size++;
                    info->count++;
                    info->sum += (j + 1);
                }
            }
            
            info->product = multiply_list(info->locations, info->locations_size);
            info->max_location = find_max(info->locations, info->locations_size);
            
            table.size++;
        }
    }
    
    return table;
}

// Function to decompress text
char* decompress(EncodingTable* table) {
    // Find maximum position to determine output length
    int max_pos = 0;
    for (int i = 0; i < table->size; i++) {
        if (table->info[i].max_location > max_pos) {
            max_pos = table->info[i].max_location;
        }
    }
    
    // Create result array
    char* result = (char*)malloc((max_pos + 1) * sizeof(char));
    memset(result, ' ', max_pos); // Fill with spaces
    result[max_pos] = '\0';
    
    // Reconstruct the original text
    for (int i = 0; i < table->size; i++) {
        char c = table->chars[i];
        EncodingInfo* info = &table->info[i];
        
        for (int j = 0; j < info->locations_size; j++) {
            result[info->locations[j] - 1] = c; // Convert to 0-based index
        }
    }
    
    return result;
}

// Function to free encoding table memory
void free_encoding_table(EncodingTable* table) {
    for (int i = 0; i < table->size; i++) {
        free(table->info[i].locations);
    }
    free(table->chars);
    free(table->info);
}

// Function to verify compression results
void verify_compression(const char* original, EncodingTable* table, const char* decompressed) {
    printf("Original text length: %zu\n", strlen(original));
    printf("Compressed text length: %d\n", table->size);
    printf("Compression ratio: %.2f%%\n", (float)table->size / strlen(original) * 100);
    printf("Successful roundtrip: %s\n", strcmp(original, decompressed) == 0 ? "Yes" : "No");
    printf("\nOriginal text: %s\n", original);
    printf("Compressed chars: ");
    for (int i = 0; i < table->size; i++) {
        printf("%c", table->chars[i]);
    }
    printf("\nDecompressed text: %s\n", decompressed);
}

int main() {
    const char* test_text = "WHO IS PROMISSING WHO";
    
    // Compress
    EncodingTable table = compress(test_text);
    
    // Decompress
    char* decompressed = decompress(&table);
    
    // Verify results
    verify_compression(test_text, &table, decompressed);
    
    // Clean up
    free_encoding_table(&table);
    free(decompressed);
    
    return 0;
}