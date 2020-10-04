.text
.global count_det

count_det:
        push {r4}
        push {r5}
        push {r6}
        push {r7}
        push {r8}
        push {r9}
        push {r10}
        ldr r1, [r0], #4
        ldr r2, [r0], #4
        ldr r3, [r0], #4
        ldr r4, [r0], #4
        ldr r5, [r0], #4
        ldr r6, [r0], #4
        ldr r7, [r0], #4
        ldr r8, [r0], #4
        ldr r9, [r0], #4
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
        pop {r10}
        pop {r9}
        pop {r8}
        pop {r7}
        pop {r6}
        pop {r5}
        pop {r4}
        bx lr