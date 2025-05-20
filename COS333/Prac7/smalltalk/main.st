Object subclass: FamilyMember [
    | expenses savings |
    
    <comment: 'A FamilyMember represents a person in a family with expenses and savings.'>

    initialize: expenseAmount [
        expenses := expenseAmount asFloat.
        savings := 0.0.
        ^self
    ]
    
    expenses [
        ^expenses
    ]
    
    savings [
        ^savings
    ]
    
    monthEnd [
        savings := savings - expenses
    ]
]

FamilyMember subclass: Teenager [
    | pocketMoney |
    
    <comment: 'A Teenager is a FamilyMember who receives pocket money.'>

    initialize: pocketMoneyAmount expenses: expenseAmount [
        super initialize: expenseAmount.
        pocketMoney := pocketMoneyAmount asFloat.
        ^self
    ]
    
    pocketMoney [
        ^pocketMoney
    ]
    
    monthEnd [
        savings := savings + pocketMoney.
        super monthEnd
    ]
]

FamilyMember subclass: Adult [
    | salary interest |
    
    <comment: 'An Adult is a FamilyMember who earns a salary and interest on savings.'>

    initialize: salaryAmount interest: interestRate expenses: expenseAmount [
        super initialize: expenseAmount.
        salary := salaryAmount asFloat.
        interest := interestRate asFloat.
        ^self
    ]
    
    salary [
        ^salary
    ]
    
    interest [
        ^interest
    ]
    
    monthEnd [
        | interestEarned |
        interestEarned := savings * (interest / 100.0).
        savings := savings + interestEarned.
        savings := savings + salary.
        super monthEnd
    ]
]

Object subclass: Family [
    | members |
    
    <comment: 'A Family manages a collection of FamilyMembers.'>

    initialize [
        members := OrderedCollection new.
        ^self
    ]
    
    addMember: aMember [
        members size >= 2 ifTrue: [
            self error: 'Cannot add more than two family members'
        ].
        members add: aMember.
        ^self
    ]
    
    payDay [
        members do: [:member | member monthEnd]
    ]
    
    printSavings [
        members do: [:member |
            Transcript show: member class name;
                      show: ': ';
                      show: member savings printString;
                      cr
        ]
    ]
]

"Test code with user input"
| family response type pocketMoney salary interest expenses |

"Create a new family"
family := Family new initialize.

"Add two family members"
2 timesRepeat: [
    Transcript show: 'Enter a teenager or adult? '.
    type := stdin nextLine.
    
    (type = 't') ifTrue: [
        Transcript show: 'Enter pocket money: '.
        pocketMoney := stdin nextLine asNumber.
        Transcript show: 'Enter expenses: '.
        expenses := stdin nextLine asNumber.
        
        family addMember: (Teenager new initialize: pocketMoney expenses: expenses)
    ] ifFalse: [
        Transcript show: 'Enter salary: '.
        salary := stdin nextLine asNumber.
        Transcript show: 'Enter interest rate (%): '.
        interest := stdin nextLine asNumber.
        Transcript show: 'Enter expenses: '.
        expenses := stdin nextLine asNumber.
        
        family addMember: (Adult new initialize: salary interest: interest expenses: expenses)
    ].
].

"Initial savings"
Transcript cr; show: 'Initial savings:'; cr.
family printSavings.

"Simulate months until user wants to stop"
[
    Transcript cr; show: 'Continue to next month? (y/n): '.
    response := stdin nextLine.
    response = 'y'
] whileTrue: [
    family payDay.
    Transcript show: 'Current savings:'; cr.
    family printSavings
]. 