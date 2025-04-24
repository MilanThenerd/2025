program Stats
    implicit none
    real :: arr(3)
    real :: range, mean, variance

    call readData(arr)
    range = findRange(arr)
    mean = findMean(arr)
    variance = findVariance(arr)

    print *, "Range: ", range
    print *, "Mean: ", mean
    print *, "Variance: ", variance

contains

    subroutine readData(arr)
        real, intent(out) :: arr(3)
        integer :: i

        print *, "Enter 3 floating point values:"
        do i = 1, 3
            read(*,*) arr(i)
        end do
    end subroutine readData

    function findRange(arr) result(range)
        real, intent(in) :: arr(3)
        real :: range
        real :: minVal, maxVal
        integer :: i

        minVal = arr(1)
        maxVal = arr(1)

        do i = 2, 3
            if (arr(i) < minVal) minVal = arr(i)
            if (arr(i) > maxVal) maxVal = arr(i)
        end do

        range = maxVal - minVal
    end function findRange

    function findMean(arr) result(mean)
        real, intent(in) :: arr(3)
        real :: mean
        mean = sum(arr) / 3.0
    end function findMean

    function findVariance(arr) result(variance)
        real, intent(in) :: arr(3)
        real :: variance
        real :: mean
        real :: sumSquares
        integer :: i

        mean = findMean(arr)
        sumSquares = 0.0

        do i = 1, 3
            sumSquares = sumSquares + (arr(i) - mean)**2
        end do

        variance = sumSquares / 2.0  ! n-1 = 3-1 = 2
    end function findVariance

end program Stats
