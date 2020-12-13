int
bitcount(STYPE value)
{
    UTYPE check_size = 1;
    int type_bits = 0;
    while (check_size) {
        if (check_size & value) {
            type_bits++;
        }
        check_size <<= 1;
    }
    return type_bits;
}
