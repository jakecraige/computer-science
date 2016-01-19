s = "azcbobobegghakl"

num_bobs = 0
s_len = len(s)
for (idx, c) in enumerate(s):
    if (s_len - idx) <= 2:
        continue
    maybeBob = c + s[idx + 1] + s[idx + 2]
    if maybeBob == "bob":
        num_bobs += 1

print("Number of times bob occurs is: " + str(num_bobs))
