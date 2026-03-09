# Contributing to ByteWeasel

Thank you for considering contributing to ByteWeasel!

## Philosophy

ByteWeasel exists to make VM development accessible. The goal is a VM framework where any intermediate C programmer can understand the code and extend it without needing to understand thousands of lines of complex code.

When contributing, keep these principles in mind:
- Simplicity over cleverness - Clear code beats clever code
- Hackability - Make it easy for users to extend and modify
- Minimal dependencies - Pure C, minimal external requirements
- Good documentation - Code should be self-explanatory, but comments help

## How Can I Contribute?

### Reporting Bugs

Open an issue with:
- Clear title
- Steps to reproduce with minimal code example
- Expected vs actual behavior
- Environment (OS, compiler version)

### Suggesting Features

Open an issue describing:
- What feature you'd like to see
- What problem does it solve
- Implementation ideas (optional)

### Your First Contribution

Look for issues labeled:
- `good first issue` - Small, well-defined tasks
- `help wanted` - We'd love help with these
- `documentation` - Improve docs, examples, or comments

Easy starter contributions:
- Add example projects showing ByteWeasel in action
- Improve documentation or code comments
- Add utility functions to the framework
- Optimize existing code
- Fix typos in documentation

## Pull Request Process

### Before You Start

1. Check existing issues/PRs - Someone might already be working on it
2. Open an issue first for big changes - Let's discuss the approach
3. For small fixes - Just open a PR

### Code Guidelines

Style:
- Use K&R style bracing
- 4 spaces for indentation (no tabs)
- Clear, descriptive variable names
- Keep functions focused and under 50 lines when possible

Commit Messages:
- Use present tense ("Add feature" not "Added feature")
- Be descriptive ("Add memory pool utility" not "updates")
- Reference issues when relevant ("Fix #42")

### Submitting a PR

1. Fork the repo and create a branch from master
2. Make your changes following the code guidelines
3. Test your changes - Make sure everything compiles and works
4. Update documentation if needed
5. Write a clear PR description: What does this do? Why? How did you test it?
6. Be patient and responsive to feedback

## Development Setup

Building:
```
cd util
./compile.sh  # Linux
compile.bat   # Windows
```

Testing:
```
gcc -Wall -Wextra -std=c99 src/byteweasel.c src/main.c -o byteweasel
./byteweasel
```

## Example Contribution Ideas

Framework Improvements:
- Performance optimizations
- Memory management utilities
- Debugging tools (bytecode disassemblers, instruction tracing)
- Platform support improvements
- API enhancements

Documentation & Examples:
- Tutorial projects (build a simple language on ByteWeasel)
- Use case examples (game scripting, config DSL, expression evaluator)
- Architecture guide explaining internals
- Performance benchmarks
- API reference documentation

Tooling:
- Build system improvements
- Testing framework
- Profiling tools
- Code generation helpers

## Code of Conduct

Be respectful. We're all here to make ByteWeasel better.

Acceptable:
- Being welcoming to newcomers
- Accepting constructive criticism gracefully
- Focusing on what's best for the project

Unacceptable:
- Harassment, insults, or personal attacks
- Trolling or inflammatory comments

## Recognition

###### _Nothing here..._

Contributors will be added to CONTRIBUTORS.md and mentioned in release notes.

Thank you for contributing!