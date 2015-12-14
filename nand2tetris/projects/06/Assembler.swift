import Foundation

extension Array {
  subscript(safe index: Int) -> Element? {
    return indices ~= index ? self[index] : .None
  }
}

typealias SymbolTable = [String : Int]

let defaultSymbols: SymbolTable = [
    "R0": 0,
    "R1": 1,
    "R2": 2,
    "R3": 3,
    "R4": 4,
    "R5": 5,
    "R6": 6,
    "R7": 7,
    "R8": 8,
    "R9": 9,
    "R10": 10,
    "R11": 11,
    "R12": 12,
    "R13": 13,
    "R14": 14,
    "R15": 15,
    "SCREEN": 16384,
    "KBD": 24576,
    "SP": 0,
    "LCL": 1,
    "ARG": 2,
    "THIS": 3,
    "THAT": 4,
    "LOOP": 4,
    "STOP": 18,
    "END": 22,
]

let jumpTable = [
    "JGT": "001",
    "JEQ": "010",
    "JGE": "011",
    "JLT": "100",
    "JNE": "101",
    "JLE": "110",
    "JMP": "111",
]

let destTable = [
    "M"  : "001",
    "D"  : "010",
    "MD" : "011",
    "A"  : "100",
    "AM" : "101",
    "AD" : "110",
    "AMD": "111",
]

let compTable = [
    // a=0
    "0"  : "0101010",
    "1"  : "0111111",
    "-1" : "0111010",
    "D"  : "0001100",
    "A"  : "0110000",
    "!D" : "0001101",
    "!A" : "0110001",
    "-D" : "0001111",
    "-A" : "0110011",
    "D+1": "0011111",
    "A+1": "0110111",
    "D-1": "0001110",
    "A-1": "0110010",
    "D+A": "0000010",
    "D-A": "0010011",
    "A-D": "0000111",
    "D&A": "0000000",
    "D|A": "0010101",

    // a=0
    "M"  : "1110000",
    "!M" : "1110001",
    "-M" : "1110011",
    "M+1": "1110111",
    "M-1": "1110010",
    "D+M": "1000010",
    "D-M": "1010011",
    "M-D": "1000111",
    "D&M": "1000000",
    "D|M": "1010101",
]

func lineIsAInstruction(line: String) -> Bool {
    return line.hasPrefix("@")
}

func lineIsLabel(line: String) -> Bool {
    return line.hasPrefix("(")
}

func removeComments(string: String) -> String {
    var result = string

    let regex = try! NSRegularExpression(pattern: "//.*", options: [])
    let match = regex.firstMatchInString(string, options: [], range: NSMakeRange(0, string.characters.count))
    if let match = match {
        result = (result as NSString).stringByReplacingCharactersInRange(match.range, withString: "") as String
    }

    return result
}

func trimWhitespaceAndComments(string: String) -> String {
    return removeComments(string).stringByTrimmingCharactersInSet(NSCharacterSet.whitespaceAndNewlineCharacterSet())
}

protocol Instruction {
    func toBinary(symbols: SymbolTable) -> String
}

extension Instruction {
    func toBinary(symbols: SymbolTable) -> String {
        return "0"
    }
}

func pad(string: String, size: Int, character: String = "0") -> String {
    var padded = string
    for _ in 0..<size - string.characters.count {
        padded = character + padded
    }
    return padded
}

struct AInstruction: Instruction {
    let address: String
    let symbol: Bool

    init(line: String) {
        self.address = line.substringFromIndex(line.startIndex.successor())
        self.symbol = Int(address) == .None
    }

    func toBinary(symbols: SymbolTable) -> String {
        let numericAddress: Int
        if symbol {
            numericAddress = symbols[address]!
        } else {
            numericAddress = Int(address)!
        }

        let binaryAddress = String(numericAddress, radix: 2)
        return pad(binaryAddress, size: 16)
    }
}

// this needs to include the jump
struct CInstruction: Instruction {
    let dest: String?
    let comp: String
    let jump: String?

    init(line: String) {
        let regex = try! NSRegularExpression(pattern: "^([A-Z]*)=?(.{1,3})(?:;(.*))?$", options: [])
        let match = regex.firstMatchInString(line, options: [], range: NSMakeRange(0, line.characters.count))

        var dict: [String: String] = [:]
        for idx in 1...3 {
            guard let range = match?.rangeAtIndex(idx) else { continue }
            guard range.location != NSNotFound else { continue }
            let substring = (line as NSString).substringWithRange(range)
            switch idx {
                case 1: dict["dest"] = substring
                case 2: dict["comp"] = substring
                case 3: dict["jump"] = substring
                default: continue
            }
        }
        self.dest = dict["dest"]
        self.comp = dict["comp"]!
        self.jump = dict["jump"]
    }

    func toBinary(symbols: SymbolTable) -> String {
        let compBinary = compTable[comp] ?? "0000000"
        let destBinary = destTable[dest ?? ""] ?? "000"
        let jumpBinary = jumpTable[jump ?? ""] ?? "000"

        return "111\(compBinary)\(destBinary)\(jumpBinary)"
    }
}

struct Label: Instruction {
    let name: String

    init(line: String) {
        let startOfLabel = line.startIndex.successor()
        let endOfLabel = line.endIndex.predecessor().predecessor()
        self.name = line[startOfLabel...endOfLabel]
    }
}

func fileContentToInstructions(content: String) -> [Instruction] {
    var allLines = [Instruction]()

    content.enumerateLines { line, stop in
        let string = trimWhitespaceAndComments(line)

        if !string.isEmpty {
            allLines.append(toInstruction(string))
        }
    }

    return allLines
}

func labelFromLine(line: String) -> String {
    let startOfLabel = line.startIndex.successor()
    let endOfLabel = line.endIndex.predecessor().predecessor()
    return line[startOfLabel...endOfLabel]
}

func populateVariables(ast: AST, symbols: SymbolTable) -> SymbolTable {
    var table = symbols
    var nextVariableAddress = 16

    for inst in ast.aInstructions {
        guard inst.symbol else { continue }
        if let _ = table[inst.address] { continue }

        table[inst.address] = nextVariableAddress
        nextVariableAddress += 1
    }

    return table
}

func populateLabels(ast: AST, symbols: SymbolTable) -> SymbolTable {
    var table = symbols
    var labelsFound = 0

    for (i, line) in ast.lines.enumerate() {
        guard let label  = line as? Label else { continue }

        table[label.name] = i - labelsFound
        labelsFound += 1
    }

    return table
}

func createSymbolTable(ast: AST) -> SymbolTable {
    var table: SymbolTable = defaultSymbols

    table = populateLabels(ast, symbols: table)
    table = populateVariables(ast, symbols: table)

    return table
}

func toInstruction(line: String) -> Instruction {
    if lineIsAInstruction(line) {
        return AInstruction(line: line)
    } else if lineIsLabel(line) {
        return Label(line: line)
    } else {
        return CInstruction(line: line)
    }
}

struct AST {
    let lines: [Instruction]
    let instructions: [Instruction]
    let aInstructions: [AInstruction]

    init(path: String) {
        let content = (try! NSString(contentsOfFile: path, encoding: NSUTF8StringEncoding)) as String
        self.lines = fileContentToInstructions(content)
        self.instructions = self.lines.filter { !($0 is Label) }
        self.aInstructions = self.instructions.filter { $0 is AInstruction }.map { $0 as! AInstruction }
    }
}

struct Parser {
    let ast: AST
    let symbolTable: SymbolTable

    init(path: String) {
        self.ast = AST(path: path)
        self.symbolTable = createSymbolTable(self.ast)
    }

    func debugPrint() {
        for node in ast.instructions {
            print("\(node.toBinary(symbolTable)): \(node)")
        }
    }

    func writeBinaryToPath(path: String) throws {
        let binaryInstructions = ast.instructions.map { $0.toBinary(self.symbolTable) }
        let instructions = binaryInstructions.joinWithSeparator("\n") + "\n"
        try instructions.writeToFile(path, atomically: true, encoding: NSUTF8StringEncoding)
    }
}

if Process.arguments.count < 2 {
    print("Error: You must pass in a path of .asm file to assemble.")
    print("Usage: swift Assembler.swift path/to/file.asm")
    exit(1)
}

let parser = Parser(path: Process.arguments[1]);
do {
    let path = "a.hack"
    try parser.writeBinaryToPath(path)
    print("Output written to: \(path)")
    exit(0)
} catch {
    print("Error writing file: \(error)")
    exit(1)
}
