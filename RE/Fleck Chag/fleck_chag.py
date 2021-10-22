def flag_check():
    flag = input("flag pls\n")
    if len(flag) !=  int(flag[8]) * 6:
        return False
    elif flag[4] != '{':
        return False
    elif flag[0] != flag[-3]:
        return False
    elif flag[6] != flag[-8]:
        return False
    elif flag[2].lower() != flag[-5] and flag[13] != flag[11]:
        return False
    elif flag.index("f") != 7:
        return False
    elif flag[9] != "_" and flag[-6] != "_":
        return False
    elif flag[-2] != 'g':
        return False
    elif int(flag[8]) + int(flag[-8]) != 4:
        return False
    elif flag.index('s') != flag.index('f') + flag.index('{'):
        return False
    else:
        return True

print(flag_check())
