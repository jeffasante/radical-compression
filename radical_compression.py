class RadicalCompressor:
    def __init__(self):
        # Initialize encoding table for all ASCII printable characters
        self.char_map = {chr(i): i for i in range(32, 127)}
    
    def compress(self, plaintext):
        """
        Compress the input text using the radical compression algorithm.
        Returns the compressed text and encoding table.
        """
        if not plaintext:
            return "", {}
            
        # Step 1: Serialize and create character mapping
        serialized = list(enumerate(plaintext, 1))
        unique_chars = dict.fromkeys(plaintext)
        
        # Step 2: Calculate encoding metrics for each unique character
        encoding_table = {}
        for char in unique_chars:
            positions = [pos for pos, c in serialized if c == char]
            if not positions:
                continue
                
            encoding_table[char] = {
                'count': len(positions),
                'locations': positions,
                'product': self._multiply_list(positions),
                'sum': sum(positions),
                'max_location': max(positions)
            }
        
        # Step 3: Create compressed string (unique characters only)
        compressed = ''.join(encoding_table.keys())
        
        return compressed, encoding_table
    
    def decompress(self, compressed_text, encoding_table):
        """
        Decompress the text using the provided encoding table.
        """
        if not compressed_text or not encoding_table:
            return ""
            
        # Find the maximum position to determine output length
        max_pos = max(info['max_location'] for info in encoding_table.values())
        result = [''] * max_pos
        
        # Reconstruct the original text using the encoding table
        for char, info in encoding_table.items():
            for pos in info['locations']:
                result[pos-1] = char
                
        return ''.join(result)
    
    def _multiply_list(self, lst):
        """Helper function to multiply all numbers in a list."""
        product = 1
        for num in lst:
            product *= num
        return product
    
    def _get_factors(self, n):
        """Helper function to get all factors of a number."""
        factors = []
        for i in range(1, n + 1):
            if n % i == 0:
                factors.append(i)
        return factors

def verify_compression(original, compressed, decompressed):
    """Helper function to verify compression results."""
    print(f"Original text length: {len(original)}")
    print(f"Compressed text length: {len(compressed)}")
    print(f"Compression ratio: {len(compressed)/len(original):.2%}")
    print(f"Successful roundtrip: {original == decompressed}")
    print(f"\nOriginal text: {original}")
    print(f"Compressed text: {compressed}")
    print(f"Decompressed text: {decompressed}")

# Example usage
if __name__ == "__main__":
    compressor = RadicalCompressor()
    
    # Test with the example from the paper
    test_text = "WHO IS PROMISSING WHO"
    
    # Compress
    compressed, encoding_table = compressor.compress(test_text)
    
    # Decompress
    decompressed = compressor.decompress(compressed, encoding_table)
    
    # Verify results
    verify_compression(test_text, compressed, decompressed)