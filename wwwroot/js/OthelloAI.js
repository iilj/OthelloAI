class OthelloAI {
    constructor(instance) {
        this.instance = instance;
        this.isRunning = false;
    }
    int2ar(num) {
        return [...Array(32).keys()].map((value) => ((num >> (31 - value)) & 1));
    }
    nums2ar(num0, num1) {
        const ar0 = this.int2ar(num0);
        const ar1 = this.int2ar(num1);
        return ar0.concat(ar1);
    }
    get black() {
        return this.nums2ar(this.instance.getBlack0(), this.instance.getBlack1());
    }
    get white() {
        return this.nums2ar(this.instance.getWhite0(), this.instance.getWhite1());
    }
    get player() {
        return this.nums2ar(this.instance.getPlayer0(), this.instance.getPlayer1());
    }
    get ai() {
        return this.nums2ar(this.instance.getAI0(), this.instance.getAI1());
    }
    get legalMove() {
        return this.nums2ar(this.instance.getLegalMove0(), this.instance.getLegalMove1());
    }
    get playerStoneCount() {
        return this.instance.getPlayerStoneCount();
    }
    get AIStoneCount() {
        return this.instance.getAIStoneCount();
    }
    get active() {
        return (this.instance != null);
    }
    // -1
    setPlayerTurn(turn) {
        return this.instance.setPlayerTurn(turn);
    }
    calcLegalMove() {
        this.instance.calcLegalMove();
    }
    isPlayable() {
        return this.instance.isPlayable();
    }
    isPlayersTurn() {
        return this.instance.isPlayersTurn();
    }
    // 'e6'
    addStone(place) {
        return this.instance.addStone(place);
    }
    addStoneAI() {
        return this.instance.addStoneAI();
    }

    sleep(sec) {
        return new Promise(resolve => {
            setTimeout(() => {
                resolve('resolved');
            }, sec);
        });
    }
    next() {
        this.instance.next();
    }
    delete() {
        this.instance.delete();
    }

    colrow2label(col, row) {
        return "abcdefgh"[col] + "12345678"[row];
    }

    async query(col, row, renderer) {
        if (this.isRunning) return;
        this.isRunning = true;
        if (this.active && this.isPlayable() && this.isPlayersTurn()) {
            const target = this.colrow2label(col, row);
            console.log(target);
            if (!this.addStone(target)) {
                console.log("addStone failed");
                // renderer.appendLog("Invalid stone location specified.");
                this.isRunning = false;
                return;
            }
            renderer.appendLog(`You > ${target}`);
            this.next();
            this.calcLegalMove();
            renderer.update();
            renderer.updatePlayersTurn(this.isPlayersTurn());
            renderer.printScore();

            if (this.isPlayersTurn()) {
                renderer.appendLog(`AI > [skipped]`);
            } else {
                await this.waitForAI(renderer);
            }
        }
        this.isRunning = false;
    }

    async waitForAI(renderer) {
        if (!this.active) return;
        while (this.isPlayable() && !this.isPlayersTurn()) {
            await this.sleep(1000);
            const addedLabel = this.addStoneAI();
            renderer.appendLog(`AI > ${addedLabel}`);
            this.next();
            this.calcLegalMove();
            renderer.update();
            renderer.updatePlayersTurn(this.isPlayersTurn());
            renderer.printScore();
            if (!this.isPlayersTurn()) {
                renderer.appendLog(`You > [skipped]`);
            }
        }
    }
}
