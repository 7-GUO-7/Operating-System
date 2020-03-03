import threading

def sort(nums, head, tail):
    if head+1 == tail or head == tail or head > tail:
        return
    mid = int((head+tail)/2)

    thread1 = threading.Thread(target=sort, args=(nums, head, mid))
    thread1.start()
    thread2 = threading.Thread(target=sort, args=(nums, mid, tail))
    thread2.start()
    thread1.join()
    thread2.join()

    nums.sort()

    return



def main():
    nums = [56, 234, 53, 633, 1, 6, 34, 763, 12, 5, 3245, 2352352, 4]
    sort(nums, 0, len(nums))
    for num in nums:
        print(num, end=" ")


if __name__ == "__main__":
    main()
