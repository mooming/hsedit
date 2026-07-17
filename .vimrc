" =============================================================================
" Vim Configuration for C++ Development
" =============================================================================

" -----------------------------------------------------------------------------
" Plugin Management (Vim8+)
" -----------------------------------------------------------------------------
set nocompatible               " Enable Vim features
filetype plugin indent on      " Enable filetype detection
syntax enable                  " Enable syntax highlighting

" -----------------------------------------------------------------------------
" Basic Settings
" -----------------------------------------------------------------------------
set number                     " Show line numbers
set relativenumber             " Relative line numbers
set tabstop=4                  " Tab width = 4 spaces
set shiftwidth=4               " Indent width = 4 spaces
set expandtab                  " Use spaces instead of tabs
set autoindent                 " Copy indent from current line
set smartindent                " Smart auto-indenting
set backspace=indent,eol,start " Backspace behavior
set undofile                   " Persistent undo
set undodir=~/.vim/undodir     " Custom undo directory

" -----------------------------------------------------------------------------
" Search Settings
" -----------------------------------------------------------------------------
set hlsearch                   " Highlight search results
set incsearch                  " Incremental search
set ignorecase                 " Case-insensitive search
set smartcase                  " Case-sensitive if uppercase used
set gdefault                   " Global substitute

" -----------------------------------------------------------------------------
" File Type Settings
" -----------------------------------------------------------------------------
filetype plugin indent on
autocmd FileType cpp setlocal tabstop=4 shiftwidth=4 expandtab

" -----------------------------------------------------------------------------
" Leader Key (must be before any mappings that use <leader>)
" -----------------------------------------------------------------------------
let mapleader = ","            " Use comma as leader key

" -----------------------------------------------------------------------------
" C++ Specific Settings
" -----------------------------------------------------------------------------
" Add ctags file location
set tags+=~/.tags
set tags+=./tags               " Project-level tags

" Quick jump to tags
nnoremap <leader>gd :tag<CR>
nnoremap <leader>gn :tnext<CR>
nnoremap <leader>gp :tprevious<CR>

" -----------------------------------------------------------------------------
" coc.nvim Configuration (Language Server)
" -----------------------------------------------------------------------------
let g:coc_config_home = $HOME . '/.vim/coc-settings.json'
let g:coc_global_extensions = ['coc-clangd']

" LSP Key Mappings (with leader prefix to avoid conflicts)
nnoremap <silent> <leader>gd <Plug>(coc-definition)
nnoremap <silent> <leader>gy <Plug>(coc-type-definition)
nnoremap <silent> <leader>gi <Plug>(coc-implementation)
nnoremap <silent> <leader>gr <Plug>(coc-references)
nnoremap <silent> <leader>K <Plug>(coc-hover)
nnoremap <silent> <C-k> <Plug>(coc-signature-help)
nnoremap <silent> <leader>rn <Plug>(coc-rename)
nnoremap <silent> <leader>a <Plug>(coc-action)
nnoremap <silent> [d <Plug>(coc-diagnostic-prev)
nnoremap <silent> ]d <Plug>(coc-diagnostic-next)

" -----------------------------------------------------------------------------
" Syntastic Configuration (Linting)
" -----------------------------------------------------------------------------
let g:syntastic_cpp_check_header = 1
let g:syntastic_cpp_check_command = 'clang++'
let g:syntastic_cpp_compiler = 'clang++'
let g:syntastic_cpp_compiler_options = '-std=c++17 -Wall -Wextra'
let g:syntastic_error_symbol = '✗'
let g:syntastic_warning_symbol = '⚠'
let g:syntastic_style_error_symbol = '●'
let g:syntastic_style_warning_symbol = '◐'

" -----------------------------------------------------------------------------
" Build and Run Shortcuts (C++)
" -----------------------------------------------------------------------------
" Read build flags from .build_flags file if it exists
function! GetBuildFlags()
    if filereadable('.build_flags')
        return readfile('.build_flags')[0]
    endif
    return '-lncurses'  " Default flags
endfunction

" Compile current file
nnoremap <leader>bb :w !clang++ -std=c++17 -Wall -Wextra -g % <GetBuildFlags()> -o %<.out && ./%<.out <CR>

" Compile with default flags (no extra libs)
nnoremap <leader>bd :w !clang++ -std=c++17 -Wall -Wextra -g % -o %<.out <CR>

" Run current file (recompile with project flags)
nnoremap <leader>rr :w !clang++ -std=c++17 -Wall -Wextra -g % <GetBuildFlags()> -o %<.out && ./%<.out <CR>

" Just run (if already compiled)
nnoremap <leader>rx :!./%<.out <CR>

" Build all files in current directory (if Makefile exists)
nnoremap <leader>bm :!make <CR>

" -----------------------------------------------------------------------------
" Debug Configuration
" -----------------------------------------------------------------------------
" GDB debugging setup
let g:gdb_use_valgrind = 0
let g:gdb_thread_tooltips = 1
nnoremap <leader>db :Gdb <CR>
nnoremap <leader>dr :GdbContinue <CR>
nnoremap <leader>ds :GdbStep <CR>
nnoremap <leader>do :GdbOver <CR>
nnoremap <leader>dt :GdbFinish <CR>
nnoremap <leader>dc :GdbStop <CR>

" -----------------------------------------------------------------------------
" Navigation
" -----------------------------------------------------------------------------
" Quickly open project files
nnoremap <leader>ff :find . -name '*.cpp' -o -name '*.h' -o -name '*.hpp' <CR>

" Toggle comments (// for C++)
augroup CppCommentFix
    autocmd!
    autocmd FileType cpp nnoremap <buffer> <leader>cc :s/^/\/\//<CR>
    autocmd FileType cpp nnoremap <buffer> <leader>cu :s/^\/\///<CR>
augroup END

" -----------------------------------------------------------------------------
" Editor Experience
" -----------------------------------------------------------------------------
set showmatch                    " Show matching brackets
set matchtime=2                  " Time to show match in ms
set scrolloff=8                  " Keep 8 lines above/below cursor
set sidescrolloff=5              " Keep 5 columns left/right
set cursorline                   " Highlight current line
set cursorcolumn                 " Highlight current column
set wildmenu                     " Enhanced command completion
set wildmode=list:longest,full   " Command completion mode
set clipboard=unnamedplus        " Share clipboard with system
set foldmethod=indent            " Fold by indentation
set foldlevelstart=10            " Open all folds by default

" -----------------------------------------------------------------------------
" Visual Improvements
" -----------------------------------------------------------------------------
set colorcolumn=80               " Visual guide at column 80
hi ColorColumn ctermbg=0         " Subtle background for colorcolumn
set list                         " Show tabs and trailing spaces
set listchars=tab:→·,trail:·     " Custom list characters
set conceallevel=0               " Don't hide anything in code

" -----------------------------------------------------------------------------
" Status Line Enhancement
" -----------------------------------------------------------------------------
set laststatus=2                 " Always show status line
set statusline=%f%h%m%r%w%y%b    " File info
set statusline+=\ %="              " Separator
set statusline+=\ %p%%             " Position in file

" -----------------------------------------------------------------------------
" Mouse Support
" -----------------------------------------------------------------------------
set mouse=a                      " Enable mouse in all modes

" -----------------------------------------------------------------------------
" Terminal Integration
" -----------------------------------------------------------------------------
" Open terminal in bottom split
nnoremap <leader>tt :sp<CR>:terminal<CR>

" Switch between normal and terminal mode easily
tnoremap <C-j> <C-\><C-n>
tnoremap <C-k> <C-\><C-n>

" -----------------------------------------------------------------------------
" Git Integration (Fugitive)
" -----------------------------------------------------------------------------
nnoremap <leader>gs :Gstatus <CR>
nnoremap <leader>gd :Gdiff <CR>
nnoremap <leader>gc :Gcommit <CR>

" -----------------------------------------------------------------------------
" Coc.nvim Commands
" -----------------------------------------------------------------------------
" Format code
nnoremap <leader>fmt :CocCommand clangd.formatSelection <CR>
nnoremap <leader>f <Plug>(coc-format)

" Show diagnostics
nnoremap <leader>dd :CocList diagnostics <CR>

" Show symbols
nnoremap <leader>ss :CocList symbols <CR>

" Show workspace symbols
nnoremap <leader>sw :CocList workspace/symbols <CR>

" Show commands
nnoremap <leader>sc :CocList commands <CR>

" -----------------------------------------------------------------------------
" Quickrun Configuration
" -----------------------------------------------------------------------------
" Add quickrun mapping if plugin is available
if executable('quickrun')
    nnoremap <leader>rq :QuickRun <CR>
endif

" =============================================================================
" Installation Commands
" =============================================================================
" Run these commands after sourcing this file:
"
" :PlugInstall (if using vim-plug)
" Or manually install via:
"   :CocInstall coc-clangd
"
" Generate tags:
"   ctags -R .
"
" =============================================================================
