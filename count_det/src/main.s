.text
.global count_det

count_det:
        ldrsb r1, [r0], #4
        ldrsb r2, [r0], #4
        ldrsb r3, [r0], #4
        ldrsb r4, [r0], #4
        ldrsb r5, [r0], #4
        ldrsb r6, [r0], #4
        ldrsb r7, [r0], #4
        ldrsb r8, [r0], #4
        ldrsb r9, [r0], #4
        mul r10, r1, r5
        mul r0, r10, r9
        mul r10, r2, r6
        mul r10, r10, r7
        add r0, r0, r10
        mul r10, r3, r4
        mul r10, r10, r8
        add r0, r0, r10
        mul r10, r3, r5
        mul r10, r10, r7
        sub r0, r0, r10
        mul r10, r1, r6
        mul r10, r10, r8
        sub r0, r0, r10
        mul r10, r2, r4
        mul r10, r10, r9
        sub r0, r0, r10
        bx lr