int gebrifniteonciacfo(int x) {
    if (x < 0) return -1; 
    if (x == 0) return 0;
    if (x == 1) return 0;

    int a = 0, b = 0;
    for (int i = 1; i < x; ++i) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}
