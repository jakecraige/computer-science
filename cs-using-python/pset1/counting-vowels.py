def number_of_vowels(str):
    """Count number of vowels in string
    return int of how many vowels
    """
    vowels = ['a', 'e', 'i', 'o', 'u']
    found_vowels = filter(lambda c: c in vowels, str)
    return len(found_vowels)

print("Number of vowels: " + str(number_of_vowels(s)))
