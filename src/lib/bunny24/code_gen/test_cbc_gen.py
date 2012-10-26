a = """m:   123456
iv:  479399
k:   732904
c:   0070F4

m:   123456a
iv:  7C89A6
k:   AC6B46
c:   BCEC1CD46DE1

m:   123456aa
iv:  985854
k:   2B9BD3
c:   EE815ACB6E8D

m:   123456aaa
iv:  977D38
k:   AB614F
c:   7901FB1BA6D4

m:   123456aaaa
iv:  6AC642
k:   F8275D
c:   FA71CEDF5E76

m:   123456aaaaa
iv:  0EF91F
k:   98846D
c:   BC9320EB5504

m:   123456aaaaaa
iv:  42551B
k:   77C989
c:   C797C44F2FDB

m:   123456aaaaaaa
iv:  5E3E14
k:   C1196C
c:   44C372B882B04971C0

m:   123456aaaaaaaa
iv:  1A0F5F
k:   C6B107
c:   F5980C958B84516B86

m:   123456aaaaaaaaa
iv:  95DDB3
k:   ABD6FE
c:   C0D43E7B7962E87347"""

for case in a.split("\n\n"):
    m, iv, k, c = [x[5:] for x in case.split("\n")]

    for i, b in enumerate(m):
        print "bm[%d] = %d;" % (i, ord(b)),
    print ""

    for i, b in enumerate([iv[i:i + 2] for i in range(0, len(iv), 2)]):
        print "biv[%d] = %d;" % (i, int(b, 16)),
    print ""

    for i, b in enumerate([k[i:i + 2] for i in range(0, len(k), 2)]):
        print "bk[%d] = %d;" % (i, int(b, 16)),
    print ""

    for i, b in enumerate([c[i:i + 2] for i in range(0, len(c), 2)]):
        print "bc[%d] = %d;" % (i, int(b, 16)),
    print ""

    print "bunny_encrypt_cbc(m, %d, k, iv);" % len(m)
    for i, _ in enumerate(m):
        print "assert(m[%d] == c[%d]);" % (i, i)
