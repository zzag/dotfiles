import React from "react";
import { Button, Table } from "react-bootstrap";


class App extends React.Component
{
    constructor(props)
    {
        super(props);
    }

    onClick()
    {
      console.info("Holla!");
    }

    render()
    {
        return (
            <div>
              <Button bsStyle="primary" onClick={this.onClick.bind(this)}>Hello</Button>

              <Table striped bordered condensed hover>
                <thead>
                  <tr>
                    <th>#</th>
                    <th>First Name</th>
                    <th>Last Name</th>
                    <th>Username</th>
                  </tr>
                </thead>
                <tbody>
                  <tr>
                    <td>1</td>
                    <td>Mark</td>
                    <td>Otto</td>
                    <td>@mdo</td>
                  </tr>
                  <tr>
                    <td>2</td>
                    <td>Jacob</td>
                    <td>Thornton</td>
                    <td>@fat</td>
                  </tr>
                  <tr>
                    <td>3</td>
                    <td colSpan="2">Larry the Bird</td>
                    <td>@twitter</td>
                  </tr>
                </tbody>
              </Table>
            </div>
        );
    }
}


React.render(<App />, document.body);
