from random import randint

for num_test in range(3):
    test_file = open('test_cache' + str(num_test) + '.txt', 'w')

    sz_cache = randint(0, 10)
    sz_data  = randint(0, 100)

    test_file.write(str(sz_cache))
    test_file.write('\n')
    test_file.write(str(sz_data))

    for elt in range(sz_data):
        test_file.write('\n')
        test_file.write(str(randint(1, 20)))
