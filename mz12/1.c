STYPE
bit_reverse(STYPE value)
{
    UTYPE uval = (UTYPE) value;
    UTYPE ures = 0;
    for (int i = 0; i <= sizeof(UTYPE) * 8 - 1; ++i) {
        ures <<= 1;
        ures |= (uval >> i) & 1;
    }
    return (STYPE) ures;
}
