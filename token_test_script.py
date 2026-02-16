import subprocess
import re
import sys
import glob

int_codes = set()
with open("ytab.h", 'r') as ytab:
        content = ytab.read()
        for word in content.split():
                word = word.strip()
                if re.match(r"^\d{3}$", word):
                        int_codes.add(int(word))

pattern = r" ?(\d+)\s*Return Code"
found_codes = set()

files = glob.glob("./*.kt")
if not files:
       print("No test files found in current directory")
       sys.exit(1)

print("Make file:")
subprocess.run("make")

for f in files:
    result = subprocess.run(["./lex", f], capture_output = True, text = True)

    output = result.stdout

    codes = set(map(int, re.findall(pattern,output,re.MULTILINE)))
    found_codes |= codes

missing = int_codes - found_codes
print("="*100)
print("Found:", sorted(found_codes))

if missing == {258, 259}:
    print("PASS: All token codes found!")
    print("This does not include error tokens.")    
elif (missing):
    print("Missing:", sorted(missing))
    
print("="*100)

clean = subprocess.run(["make", "clean"])