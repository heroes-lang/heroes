// The kanban board's page. It keeps no state of its own but which board is
// shown: every board, column and card lives in the program, which keeps them
// in SQLite, and every change is a call over the bridge (`heroes.invoke`),
// answered by the program, after which the page asks the board again. The
// program tells the page when the board changed by an event, `board.changed`,
// so an undo or an import made anywhere redraws it.
"use strict";

(function () {
  var shown = null;
  var dragged = null;
  var previewing = 0;
  var editing = null;

  function element(name, attributes, children) {
    var made = document.createElement(name);
    Object.keys(attributes || {}).forEach(function (key) {
      if (key === "text") {
        made.textContent = attributes[key];
      } else if (key === "className") {
        made.className = attributes[key];
      } else {
        made.setAttribute(key, attributes[key]);
      }
    });
    (children || []).forEach(function (child) {
      made.appendChild(child);
    });
    return made;
  }

  function byId(id) {
    return document.getElementById(id);
  }

  function say(text, failed) {
    var status = byId("status");
    status.textContent = text;
    status.className = failed ? "status error" : "status";
  }

  // A call to the program; a refusal is told in the status line and handed
  // on, so the caller stops.
  function call(name, args) {
    return heroes.invoke(name, args).catch(function (error) {
      say(error.message + " (" + (error.code || "failed") + ")", true);
      throw error;
    });
  }

  // Asks the person for a text in the page's own dialog; resolves with the
  // text, or with null where they cancelled.
  function ask(label, value) {
    return new Promise(function (resolve) {
      var dialog = byId("ask");
      var input = byId("ask-input");
      byId("ask-label").textContent = label;
      input.value = value || "";
      dialog.onclose = function () {
        resolve(dialog.returnValue === "ok" ? input.value.trim() : null);
      };
      dialog.showModal();
      input.select();
    });
  }

  function loadBoards(wanted) {
    return call("boards.list", null).then(function (boards) {
      var select = byId("board-select");
      select.textContent = "";
      boards.forEach(function (board) {
        select.appendChild(element("option", { value: board.id, text: board.title }));
      });
      byId("empty").hidden = boards.length > 0;
      ["board-rename", "board-delete", "export", "report", "search"].forEach(function (id) {
        byId(id).disabled = boards.length === 0;
      });
      if (boards.length === 0) {
        shown = null;
        byId("board").textContent = "";
        return null;
      }
      var keep = boards.some(function (b) { return b.id === wanted; }) ? wanted : boards[0].id;
      select.value = keep;
      return showBoard(keep);
    });
  }

  function showBoard(id) {
    return call("board.get", { board: id }).then(function (board) {
      shown = board.id;
      drawBoard(board);
      byId("undo").disabled = !board.undo;
      byId("redo").disabled = !board.redo;
      byId("undo").title = board.undo ? "Undo " + board.undo : "Nothing to undo";
      byId("redo").title = board.redo ? "Redo " + board.redo : "Nothing to redo";
    });
  }

  function drawBoard(board) {
    var area = byId("board");
    area.textContent = "";
    board.columns.forEach(function (column, index) {
      area.appendChild(drawColumn(column, index, board.columns));
    });
    var title = element("input", { placeholder: "A new column", "aria-label": "A new column" });
    var add = element("button", { type: "button", text: "Add column" });
    add.addEventListener("click", function () {
      if (title.value.trim() !== "") {
        call("column.add", { board: board.id, title: title.value.trim() }).then(function () {
          say("Column added.");
        });
      }
    });
    area.appendChild(element("div", { className: "add-column" }, [title, add]));
  }

  function drawColumn(column, index, columns) {
    var cards = element("div", { className: "cards" });
    column.cards.forEach(function (card, at) {
      cards.appendChild(drawCard(card, column, at, index, columns));
    });
    var rename = element("button", { type: "button", text: "Rename", title: "Rename this column" });
    rename.addEventListener("click", function () {
      ask("Column", column.title).then(function (title) {
        if (title) {
          call("column.rename", { column: column.id, title: title });
        }
      });
    });
    var remove = element("button", { type: "button", text: "Delete", className: "danger", title: "Delete this column and its cards" });
    remove.addEventListener("click", function () {
      call("column.delete", { column: column.id }).then(function () {
        say("Column deleted; Undo brings it back.");
      });
    });
    var head = element("div", { className: "column-head" }, [
      element("h2", { text: column.title }),
      element("span", { className: "count", text: String(column.cards.length) }),
      rename,
      remove
    ]);
    var title = element("input", { placeholder: "A new card", "aria-label": "A new card in " + column.title });
    var add = element("button", { type: "button", text: "Add" });
    function addCard() {
      if (title.value.trim() !== "") {
        call("card.add", { column: column.id, title: title.value.trim(), body: "" });
      }
    }
    add.addEventListener("click", addCard);
    title.addEventListener("keydown", function (event) {
      if (event.key === "Enter") {
        addCard();
      }
    });
    var made = element("section", { className: "column", "data-column": column.id }, [
      head,
      cards,
      element("div", { className: "add-card" }, [title, add])
    ]);
    made.addEventListener("dragover", function (event) {
      if (dragged !== null) {
        event.preventDefault();
        made.classList.add("dropping");
      }
    });
    made.addEventListener("dragleave", function () {
      made.classList.remove("dropping");
    });
    made.addEventListener("drop", function (event) {
      event.preventDefault();
      made.classList.remove("dropping");
      if (dragged === null) {
        return;
      }
      var position = dropPosition(cards, event.clientY);
      var card = dragged;
      dragged = null;
      call("card.move", { card: card, column: column.id, position: position });
    });
    return made;
  }

  // The place among `cards` a card dropped at `y` takes: before the first
  // card whose middle is below it.
  function dropPosition(cards, y) {
    var children = Array.prototype.slice.call(cards.children);
    for (var i = 0; i < children.length; i += 1) {
      var box = children[i].getBoundingClientRect();
      if (y < box.top + box.height / 2) {
        return i;
      }
    }
    return children.length;
  }

  function drawCard(card, column, at, index, columns) {
    var body = element("div", { className: "markdown" });
    // The program renders the card's Markdown and cleans the HTML by an
    // allow-list; the page's own policy runs no script written in it either.
    body.innerHTML = card.html;
    var moves = element("div", { className: "moves" });
    if (index > 0) {
      var left = element("button", { type: "button", text: "←", title: "Move to " + columns[index - 1].title });
      left.addEventListener("click", function (event) {
        event.stopPropagation();
        call("card.move", { card: card.id, column: columns[index - 1].id, position: columns[index - 1].cards.length });
      });
      moves.appendChild(left);
    }
    if (at > 0) {
      var up = element("button", { type: "button", text: "↑", title: "Move up" });
      up.addEventListener("click", function (event) {
        event.stopPropagation();
        call("card.move", { card: card.id, column: column.id, position: at - 1 });
      });
      moves.appendChild(up);
    }
    if (index < columns.length - 1) {
      var right = element("button", { type: "button", text: "→", title: "Move to " + columns[index + 1].title });
      right.addEventListener("click", function (event) {
        event.stopPropagation();
        call("card.move", { card: card.id, column: columns[index + 1].id, position: columns[index + 1].cards.length });
      });
      moves.appendChild(right);
    }
    var made = element("article", { className: "card", draggable: "true", tabindex: "0", "data-card": card.id }, [
      element("h3", { text: card.title }),
      body,
      moves
    ]);
    made.addEventListener("dragstart", function (event) {
      dragged = card.id;
      made.classList.add("dragging");
      event.dataTransfer.setData("text/plain", card.title);
      event.dataTransfer.effectAllowed = "move";
    });
    made.addEventListener("dragend", function () {
      made.classList.remove("dragging");
      dragged = null;
    });
    made.addEventListener("click", function () {
      edit(card.id);
    });
    made.addEventListener("keydown", function (event) {
      if (event.key === "Enter") {
        edit(card.id);
      }
    });
    return made;
  }

  function edit(id) {
    call("card.get", { card: id }).then(function (card) {
      editing = card.id;
      byId("editor-title").value = card.title;
      byId("editor-body").value = card.body;
      byId("editor-preview").innerHTML = card.html;
      byId("editor").showModal();
    });
  }

  function preview() {
    previewing += 1;
    var mine = previewing;
    call("preview", { body: byId("editor-body").value }).then(function (answer) {
      if (mine === previewing) {
        byId("editor-preview").innerHTML = answer.html;
      }
    });
  }

  function search() {
    var query = byId("search").value.trim();
    var results = byId("results");
    if (query === "" || shown === null) {
      results.hidden = true;
      results.textContent = "";
      return;
    }
    call("search", { board: shown, query: query }).then(function (hits) {
      results.textContent = "";
      results.hidden = false;
      results.appendChild(element("strong", { text: hits.length + (hits.length === 1 ? " card" : " cards") + " for “" + query + "”" }));
      var list = element("ol");
      hits.forEach(function (hit) {
        var item = element("li", {}, [
          element("a", { href: "#", text: hit.title }),
          element("span", { className: "where", text: " in " + hit.column })
        ]);
        item.firstChild.addEventListener("click", function (event) {
          event.preventDefault();
          edit(hit.card);
        });
        list.appendChild(item);
      });
      results.appendChild(list);
    });
  }

  function wire() {
    byId("board-select").addEventListener("change", function (event) {
      showBoard(event.target.value);
    });
    byId("board-new").addEventListener("click", function () {
      ask("Board", "").then(function (title) {
        if (title) {
          call("board.create", { title: title }).then(function (board) {
            return loadBoards(board.id);
          });
        }
      });
    });
    byId("board-rename").addEventListener("click", function () {
      var select = byId("board-select");
      ask("Board", select.options[select.selectedIndex].textContent).then(function (title) {
        if (title) {
          call("board.rename", { board: shown, title: title });
        }
      });
    });
    byId("board-delete").addEventListener("click", function () {
      call("board.delete", { board: shown }).then(function () {
        say("Board deleted; Undo brings it back.");
      });
    });
    byId("undo").addEventListener("click", function () {
      call("undo", null).then(function (done) {
        say("Undid " + done.label + ".");
      });
    });
    byId("redo").addEventListener("click", function () {
      call("redo", null).then(function (done) {
        say("Redid " + done.label + ".");
      });
    });
    byId("export").addEventListener("click", function () {
      call("export.file", { board: shown }).then(function (answer) {
        say(answer.saved ? "Board saved." : "Nothing saved.");
      });
    });
    byId("import").addEventListener("click", function () {
      call("import.file", null).then(function (answer) {
        if (answer.board) {
          say("Board imported.");
          return loadBoards(answer.board);
        }
        say("Nothing imported.");
        return null;
      });
    });
    byId("report").addEventListener("click", function () {
      call("report", { board: shown }).then(function (answer) {
        window.location.assign(answer.path);
      });
    });
    byId("search").addEventListener("input", search);
    byId("editor-body").addEventListener("input", preview);
    byId("editor").addEventListener("close", function () {
      var dialog = byId("editor");
      if (dialog.returnValue === "save" && editing !== null) {
        call("card.edit", { card: editing, title: byId("editor-title").value.trim(), body: byId("editor-body").value });
      }
      editing = null;
    });
    byId("editor-delete").addEventListener("click", function () {
      var id = editing;
      editing = null;
      byId("editor").close("cancel");
      call("card.delete", { card: id }).then(function () {
        say("Card deleted; Undo brings it back.");
      });
    });
    document.addEventListener("keydown", function (event) {
      var primary = event.metaKey || event.ctrlKey;
      if (primary && (event.key === "z" || event.key === "Z") && document.activeElement.tagName !== "TEXTAREA" && document.activeElement.tagName !== "INPUT") {
        event.preventDefault();
        byId(event.shiftKey ? "redo" : "undo").click();
      }
    });
    // Every change redraws the list of boards too, so a board renamed,
    // made, deleted or put back by undo shows in the selector at once.
    heroes.on("board.changed", function (change) {
      Promise.resolve(loadBoards(shown !== null ? shown : change.board)).then(search);
    });
  }

  // The board to show first: the one the report links back to, or the
  // first.
  document.addEventListener("DOMContentLoaded", function () {
    wire();
    loadBoards(new URLSearchParams(window.location.search).get("board"));
  });
})();
