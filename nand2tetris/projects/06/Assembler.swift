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

func lineIsAInstruction(line: String) -> Bool {
    return line.hasPrefix("@")
}

func lineIsCInstruction(line: String) -> Bool {
    return false
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
    func toBinary() -> String
}

extension Instruction {
    func toBinary() -> String {
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

    func toBinary() -> String {
        guard let numericAddress = Int(address) else { fatalError("Non-numeric address: \(address)") }
        let binaryAddress = String(numericAddress, radix: 2)
        return pad(binaryAddress, size: 16)
    }
}

// this needs to include the jump
struct CInstruction: Instruction {
    let dest: String
    let comp: String
    let jump: String?

    init(line: String) {
        let jmpParts = line.componentsSeparatedByString(";")
        let destParts = jmpParts[0].componentsSeparatedByString("=")

        self.dest = destParts[0]
        self.comp = destParts[safe: 1] ?? jmpParts[0]
        self.jump = jmpParts[safe: 1]
    }

    func toBinary() -> String {
        // "1-awholeslewofthings"
        return "0"
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

        table[label.name] = i + 1 - labelsFound
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
}

let parser = Parser(path: Process.arguments[1]);
// print(parser.ast.instructions)
for node in parser.ast.instructions {
    print("\(node.toBinary()): \(node)")
}
