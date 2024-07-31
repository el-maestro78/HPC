import sys

def compare_bins(bin1, bin2):
    try:
        # Open both files in binary mode
        with open(bin1, 'rb') as file1, open(bin2, 'rb') as file2:
            # Read both files in chunks
            chunk_size = 1024  # Read in 1KB chunks
            while True:
                file1_chunk = file1.read(chunk_size)
                file2_chunk = file2.read(chunk_size)
                
                # If both chunks are empty, the files are identical
                if not file1_chunk and not file2_chunk:
                    print("Files are identical.")
                    return True
                
                # If chunks differ, the files are different
                if file1_chunk != file2_chunk:
                    print("Files are different.")
                    return False
    
    except FileNotFoundError as e:
        print(f"Error: {e}")
    except PermissionError as e:
        print(f"Permission denied: {e}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

# Example usage
if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python bin_comp.py <file1> <file2>")
        sys.exit(1)
    
    file1 = sys.argv[1]
    file2 = sys.argv[2]
    
    compare_bins(file1, file2)
