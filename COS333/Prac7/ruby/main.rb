class FamilyMember
  attr_reader :expenses, :savings

  def initialize(expenses = 0)
    @expenses = expenses.to_f
    @savings = 0.0
  end

  def month_end
    @savings -= @expenses
  end
end 

class Adult < FamilyMember
  attr_reader :salary, :interest

  def initialize(salary, interest, expenses)
    super(expenses)
    @salary = salary.to_f
    @interest = interest.to_f
  end

  def month_end
    interest_earned = @savings * (@interest / 100.0)
    @savings += interest_earned
    @savings += @salary
    super
  end
end 

class Teenager < FamilyMember
  attr_reader :pocket_money

  def initialize(pocket_money, expenses)
    super(expenses)
    @pocket_money = pocket_money.to_f
  end

  def month_end
    @savings += @pocket_money
    super
  end
end 

class Family
  def initialize
    @members = []
  end

  def add_family_member(member)
    raise "Cannot add more than two family members" if @members.length >= 2
    @members << member
  end

  def pay_day
    @members.each(&:month_end)
  end

  def print_savings
    @members.each_with_index do |member, index|
      puts "#{member.class}: #{member.savings}"
    end
  end
end 

def get_member_details
  print "Enter a teenager or adult? (t/a): "
  type = gets.chomp.downcase

  case type
  when 't'
    print "Enter pocket money: "
    pocket_money = gets.to_f
    print "Enter expenses: "
    expenses = gets.to_f
    Teenager.new(pocket_money, expenses)
  when 'a'
    print "Enter salary: "
    salary = gets.to_f
    print "Enter interest (%): "
    interest = gets.to_f
    print "Enter expenses: "
    expenses = gets.to_f
    Adult.new(salary, interest, expenses)
  else
    puts "Invalid member type!"
    exit(1)
  end
end

family = Family.new

2.times do |i|
  puts "\nEnter details for family member #{i + 1}:"
  family.add_family_member(get_member_details)
end

loop do
  print "\nAnother month? (y/n): "
  break if gets.chomp.downcase != 'y'
  
  family.pay_day
  family.print_savings
end

puts "\nFinal savings:"
family.print_savings 