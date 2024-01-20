# Rename all *.c to *.C
for file in *.c; do
    mv -- "$file" "${file%.c}.C"
done

# Rename all *.h to *.H
for file in *.h; do
    mv -- "$file" "${file%.h}.H"
done