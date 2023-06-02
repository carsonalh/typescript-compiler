let a = 1;
let b = a + 1;
const c = a + b;

interface Abc123 {
    a: string;
    b: number;
    c: any;
}

function abc123itDoesntMatterWhatICallThis(x: Abc123) {
    let a = 1;
    return x.b++ + ++a;
}
