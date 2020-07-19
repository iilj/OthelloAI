class TableRenderer {
    constructor() {
        const instance = new Module.OthelloGame();
        this.ai = new OthelloAI(instance);
        this.initLog();
        this.renderButtons();
    }

    renderButtons() {
        const uc = document.getElementById("upper-container");

        const b0 = document.createElement('button');
        b0.innerText = "Black (first)";
        b0.addEventListener("click", () => {
            this.initBoard(-1);
        })
        const b1 = document.createElement('button');
        b1.innerText = "White (second)";
        b1.addEventListener("click", () => {
            this.initBoard(1);
        });
        this.b0 = b0;
        this.b1 = b1;
        uc.appendChild(b0);
        uc.appendChild(b1);
        this.appendLog("Choose your turn.");
    }

    initLog() {
        const logwindow = document.createElement('div');
        logwindow.classList.add("log");
        document.getElementById("lower-container").appendChild(logwindow);
        this.logwindow = logwindow;
    }

    initBoard(turn) {
        const uc = document.getElementById("upper-container");
        uc.removeChild(this.b0);
        uc.removeChild(this.b1);

        console.log(this.ai.setPlayerTurn(turn));
        this.ai.calcLegalMove();

        const tbl = document.createElement('table');
        const tbdy = document.createElement('tbody');
        const cells = [];

        // header
        const header = document.createElement('tr');
        const empty = document.createElement('td');
        empty.appendChild(document.createTextNode(""));
        empty.classList.add("header");
        header.appendChild(empty);
        for (let j = 0; j < 8; ++j) {
            const td = document.createElement('td');
            td.appendChild(document.createTextNode("abcdefgh"[j]));
            td.classList.add("header");
            header.appendChild(td);
        }
        tbdy.appendChild(header);

        for (let i = 0; i < 8; ++i) {
            const tr = document.createElement('tr');
            const rownum = document.createElement('td');
            rownum.classList.add("header");
            rownum.appendChild(document.createTextNode(`${i + 1}`));
            tr.appendChild(rownum);
            for (let j = 0; j < 8; ++j) {
                const td = document.createElement('td');
                td.appendChild(document.createTextNode(''));
                td.addEventListener('click', async () => {
                    if (!this.ai.isPlayable()) return;
                    await this.ai.query(j, i, this);
                    if (!this.ai.isPlayable()) {
                        this.appendLog("Ended.");
                        this.printScore();
                        this.printResult();
                    }
                });
                tr.appendChild(td);
                cells.push(td);
            }
            tbdy.appendChild(tr);
        }
        tbl.appendChild(tbdy);
        uc.appendChild(tbl);

        this.cells = cells;
        this.tbl = tbl;
        this.update();
        this.appendLog("Initialized board.");
        this.printScore();
        this.updatePlayersTurn(this.ai.isPlayersTurn());

        if (!this.ai.isPlayersTurn()) {
            this.ai.waitForAI(this);
        }
    }

    update() {
        const tbl_blacks = this.ai.black;
        const tbl_whites = this.ai.white;
        const tbl_legal = this.ai.legalMove;
        for (let i = 0; i < 8; ++i) for (let j = 0; j < 8; ++j) {
            const idx = 8 * i + j;
            if (tbl_blacks[idx] == 1) {
                this.cells[idx].innerText = '●';
                this.cells[idx].classList.remove("whiteStone");
                this.cells[idx].classList.add("blackStone");
            } else if (tbl_whites[idx] == 1) {
                this.cells[idx].innerText = '●';
                this.cells[idx].classList.remove("blackStone");
                this.cells[idx].classList.add("whiteStone");
            } else {
                this.cells[idx].innerText = '―';
            }
            if (tbl_legal[idx] == 1) {
                this.cells[idx].classList.add("legalCell");
            } else {
                this.cells[idx].classList.remove("legalCell");
            }
        }
    }

    updatePlayersTurn(flag) {
        if (flag) {
            this.tbl.classList.remove("ai");
            this.tbl.classList.add("player");
        } else {
            this.tbl.classList.remove("player");
            this.tbl.classList.add("ai");
        }
    }

    appendLog(s) {
        const ss = document.createElement('div');
        ss.appendChild(document.createTextNode(s));
        this.logwindow.appendChild(ss);

        const scrollHeight = this.logwindow.scrollHeight;
        this.logwindow.scrollTop = scrollHeight;
    }

    printScore() {
        const player = this.ai.playerStoneCount;
        const ai = this.ai.AIStoneCount;
        this.appendLog(`[Score] You: ${player}, AI: ${ai}`);
    }

    printResult() {
        const player = this.ai.playerStoneCount;
        const ai = this.ai.AIStoneCount;
        if (player > ai) {
            this.appendLog("You win.");
            alert("You win.");
        } else if (player < ai) {
            this.appendLog("AI wins.");
            alert("AI wins.");
        } else {
            this.appendLog("Draw.");
            alert("Draw.");
        }
    }
};

var Module = {
    onRuntimeInitialized: function () {
        const renderer = new TableRenderer();
        // window.renderer = renderer;
    }
};