# cmd
> a handy tool for those who are attached to a terminal memorizing commands for common tasks!

I've made this tool to bookmark common commands typed on terminal every day, so next time I just need to remember it's alias, o get it from the list provided by the `-l` argument. 😌

![printscreen](https://github.com/dbonates/cmd/blob/master/img/print.png)

### How it works

While on terminal, fire the app with parameter `-s` followed by an alias and the command with quotes. Like this:

`cmd -s peixe "open https://peixeurbano.com.br"`

Then, it will be as an alias on your shell as soon as you refresh your `~/.bash_profile` file.

Later, you just need to type `peixe` and the related command will be fired.

### Instalation:

Just download the latest binary from releases page and drop it into the `/usr/local/bin/` and you're ready to go.

Of course, feel free to download the source and compile it from your local machine.


### Known Issues:

I couldn't manage to update the alias for current terminal window from inside the app, as the apps runs on it's own process and is unable to modify another. But once they're saved, they will be available as soon you refresh the shell by running`source ~/.bash_profile` by hand. Or of course, open fresh terminal screen and they will be there for you 😉.


---

## License

Copyright 2017 Daniel Bonates

Use of the code provided on this repository is subject to the [MIT License](http://www.opensource.org/licenses/mit-license.php).

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files 
(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION 
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
