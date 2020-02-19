import subprocess
import sys

def fa_stats(file):
    output = str (subprocess.check_output(f"./fa {file}".split()))[2:-3]
    values = [kp for item in output.split(',') for kp in item.split('=')]
    print(values)
    
if __name__ == "__main__":
    if len(sys.argv) > 1 :
        fa_stats(sys.argv[1])
