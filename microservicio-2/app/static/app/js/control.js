var hostname = window.location.hostname;
const socketEstadoLampara = new WebSocket('ws://'+hostname+'/microservicio1');
const socketPeticionEstadoLampara = new WebSocket('ws://'+hostname+'/microservicio1');
const socketControlLampara = new WebSocket('ws://'+hostname+'/microservicio1');

// para despues de conectarnos al socket para ver estado de la lampara
socketEstadoLampara.onopen = function() {
    socketEstadoLampara.send(JSON.stringify({ type: 'subscribe', channel: 'estado_lampara' }));
};
// para despues de conectarnos al socket cambiar el estado de la lampara
socketControlLampara.onopen = function() {
    socketControlLampara.send(JSON.stringify({ type: 'subscribe', channel: 'control_lampara' }));
};

socketEstadoLampara.onmessage = function(event) {
    //event.data
    // si la lampara esta encendida entonces ponemos el slide en encendido
    const data = JSON.parse(event.data);
    console.log(data.content)
    if(data.content == '1'){
        accionSlide(true);
    }else if(data.content == '0'){
        accionSlide(false);
    }
};

/* 
    Enviamos un mensaje al socket para que el Microcontrolador mande el estado a la lampara
*/
socketPeticionEstadoLampara.onopen = function() {
    socketPeticionEstadoLampara.send(JSON.stringify({ type: 'subscribe', channel: 'peticion_estado_lampara' }));
    socketPeticionEstadoLampara.send(JSON.stringify({ type: 'message', content:'1' }));
};


function accionSlide(estado){
    let contenedorSlide = document.querySelector('.contenedor-slide');
    let textoSlide = contenedorSlide.children[0];
    let slide = contenedorSlide.children[1];
    //si el estado es true ponemos el slide en encendido
    if(estado == true){
        slide.classList.remove('slide-off');
        slide.classList.add('slide-on');
        textoSlide.classList.remove('texto-apagado');
        textoSlide.classList.add('texto-encendido');
        textoSlide.innerHTML= 'Encendida';
    }else{// si es false lo ponemos en apagado
        slide.classList.remove('slide-on');
        slide.classList.add('slide-off');
        textoSlide.classList.remove('texto-encendido');
        textoSlide.classList.add('texto-apagado');
        textoSlide.innerHTML= 'Apagada';
    }
}

function slideLampara(){
    let contenedorSlide = document.querySelector('.contenedor-slide');
    let textoSlide = contenedorSlide.children[0];
    let slide = contenedorSlide.children[1];
    // si el slide esta encendido entonces lo apagamos
    if(slide.classList.contains('slide-on')== true){
        socketControlLampara.send(JSON.stringify({ type: 'message', content:'0' }));
    }else{// si el slide esta apagado entonces lo encendemos
        socketControlLampara.send(JSON.stringify({ type: 'message', content:'1' }));
    }
}