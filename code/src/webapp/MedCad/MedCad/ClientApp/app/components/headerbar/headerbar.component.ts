import { Component } from '@angular/core';
import { ModelsService } from '../../services/models/models.service'
import { Auth } from '../../services/auth/auth.service';

@Component({
    selector: 'headerbar',
    template: require('./headerbar.component.html'),
    styles: [require('./headerbar.component.scss')],
    providers: [ModelsService, Auth]
})

export class HeaderbarComponent {

    constructor(private auth: Auth) {
    }

    public openFileContextClickHandler() {
        document.getElementById('open-file-dlg').click();
    }
}
